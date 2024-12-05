#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <errno.h>
#include <sys/shm.h>  //shmget,shmat,shmdt 
#include <semaphore.h> 
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <signal.h>
#include "sock.h"
#include "bsp_types.h"
#include "OS_PROCS.h"
#include "POSAPI.h"
#include "IPC_client.h"
#include "UTILS.h"
// #include "PEDKconfig.h"
/**
 * SHM structure
 * UINT8    API return value(If client call API, this value will be 0x99)
 * UINT8    PID             (API code ex:psDEV_LAN  see OS_PROCS.h for more information)
 * UINT8    APInumber       (Order of API function. XXX_open() will be 1, and XXX_close() will be 2)
 * UINT8    ArgsNumber      (Numbers of Arguments )
 * UINT32   ArgsSize        (Input/Output arguments total size)
 * UINT8    *Args           (Input arguments)
 * 
*/
/**
 * 20220413 West Note:
 * IPC_clientHandler could be more convenient. 
 * Add a parameter as an output pointer, and the data from IPC_server will copy to the output.
 * So user will no longer copy output data from input data(*Arg) manually.
 * e.q.
 * IPC_clientHandler(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 IptSize,UINT8 *Arg,UINT8 *retval,UINT8 *optdata)
 * 
 * I figuare it out after done with 11 API... Too lazy to change code have written done.
 */
extern void	APP_main( void );
extern BSP_BOOL OS_EnableTimer1( void );
struct sockaddr_in saddr;
int sockfd;
int shmfd;
int shmTMRfd;
static sem_t *sem_A,*sem_B;
static key_t shm_key;
UCHAR *shm_data;
UCHAR *shm_timer;


void openSHM()
{
    shm_key = ftok("/", 888);
    if(shm_key < 0)
    {
        perror("get_key failed ");
        exit(1);
    }
    printf("client shm_key=%d\n", shm_key);

    shmfd = shmget(shm_key, MMAP_DATA_SIZE, 0777);
    if(shmfd < 0)
    {
        perror("creat failed ");
        exit(1);
    }

    shm_data = shmat(shmfd, NULL, 0);
    if(shm_data < 0)
    {
        perror("map failed ");
        exit(1);
    }

    sem_A = sem_open("/semA", O_CREAT, 0777, 0);
    sem_B = sem_open("/semB", O_CREAT, 0777, 0);

    memset(shm_data, 0, MMAP_DATA_SIZE); //initial shm area
}

void IPC_clientSet(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 ArgsSize, UINT8 *Arg)
{
#ifdef _DEBUG_SEMAPHORE_
    int i = 0;
    int valueOfA, valueOfB;
#endif


    shm_data[1] = PID;
    shm_data[2] = APInum;
    shm_data[3] = ArgsNum;
    memmove(&shm_data[4], &ArgsSize, 4);
    if(ArgsSize > 0)  
        memmove(&shm_data[8], Arg, ArgsSize);
    // shm_data[0]=CLEIENT_ARGS_SET;   //Set flag after arguments setting finished.
    
#ifdef _DEBUG_SEMAPHORE_
    printf(">>>shm_data from AP\n");
  
    for(i = 0 ; i < (8 + ArgsSize) ; i++)
    {
        printf("%02x ", shm_data[i]);
    }
    printf("\n");
#endif

#ifdef _DEBUG_SEMAPHORE_
    sem_getvalue(sem_A, &valueOfA);
    sem_getvalue(sem_B, &valueOfB);
    printf("[AP] sem_A = %d, sem_B = %d before sem_post(sem_A)\n", valueOfA, valueOfB);
#endif
  
    sem_post(sem_A);  //?�Ĥ@?�H?�q?��V�ާ@ (sem_A++ to signal SP)

#ifdef _DEBUG_SEMAPHORE_
    sem_getvalue(sem_A, &valueOfA);
    sem_getvalue(sem_B, &valueOfB);
    printf("[AP] sem_A = %d, sem_B = %d after sem_post(sem_A)\n", valueOfA, valueOfB);
#endif
}

void IPC_clientRead(UINT8 *ReturnValue, UINT8 *ReturnArgs, UINT32 IptSize, UINT32 OptSize)
{
#ifdef _DEBUG_SEMAPHORE_
    int i = 0;
#endif


    *ReturnValue = shm_data[0];
    if(OptSize > 0)
    memmove(ReturnArgs, &shm_data[8 + IptSize], OptSize);

#ifdef _DEBUG_SEMAPHORE_
    if(OptSize > 0)
    {
        printf(">>>ReturnArgs = ");
        
        for(i = 0 ; i < OptSize ; i++)
        {
            printf("%02x ", ReturnArgs[i]);
        }
        printf("\n");
    }
#endif 
}

void IPC_clientHandler(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval)
{
    UINT32 OptSize;
#ifdef _DEBUG_SEMAPHORE_
    int valueOfA, valueOfB;
#endif


    IPC_clientSet(PID, APInum, ArgsNum, IptSize, Arg);

#ifdef _DEBUG_SEMAPHORE_
    sem_getvalue(sem_A, &valueOfA);
    sem_getvalue(sem_B, &valueOfB);
    printf("[AP] sem_A = %d, sem_B = %d before sem_wait(sem_B)\n", valueOfA, valueOfB);
#endif

    sem_wait(sem_B);  //?�ĤG?�H?�q?��P�ާ@ (waiting here if sem_B=0, until SP finishes the target process)

#ifdef _DEBUG_SEMAPHORE_
    sem_getvalue(sem_A, &valueOfA);
    sem_getvalue(sem_B, &valueOfB);
    printf("[AP] sem_A = %d, sem_B = %d after sem_wait(sem_B)\n", valueOfA, valueOfB);
#endif

    // while(CLEIENT_ARGS_SET == shm_data[0]);   //wait until server set flag to API return value
    memmove(&OptSize, &shm_data[4], 4); //write back output arguments length from SP.
    // printf("OptSize=%d\n", OptSize);
    
#ifdef _DEBUG_SEMAPHORE_
    printf(">>>shm_data from SP\n");
    int i = 0;
    for(i = 0 ; i < (8 + IptSize + OptSize) ; i++)
    {
        printf("%02x ", shm_data[i]);
    }
    printf("\n");
#endif

#ifdef _DEBUG_SEMAPHORE_
    printf("OptSize = %d\n", OptSize);
#endif

    IPC_clientRead(retval, Arg + IptSize, IptSize, OptSize);

#ifdef _DEBUG_SEMAPHORE_
    printf("retval = %02x\n", *retval);
#endif

  memset(shm_data, 0, IptSize + OptSize + 8);   //initial shm area
}

void report(const char* msg, int terminate)
{
    perror(msg);
    if (terminate) exit(-1); /* failure */
}

//*ret - return value array
//       First byte is API return value.
//       Follow bytes are return value of input pointer if need.
UINT8 IPC_Reciever(UINT8 *ret, UINT32 reclen)
{
    UINT8 res = FALSE;


    if(read(sockfd, ret, reclen) > 0)
    {
        // printf("retval=%d\n", retvalue);
        res = TRUE;
    }
    return res;
}

UINT8 IPC_Reciever2(UINT8 *ret, UINT32 reclen)
{
    UINT8 res = FALSE;
    UINT32 recSize = 1;


    if(read(sockfd, ret, 1) > 0)
    {
        while(recSize != reclen)
        recSize += read(sockfd, ret + 1 + recSize, reclen);
        // printf("retval=%d\n", retvalue);
        res = TRUE;
    }
    return res;    
}

UINT8 IPC_Sender(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 ArgsSize, UINT8 *Arg)
{
    OS_IPCSOCKET_HEADER socketHeader;
    OS_IPCSOCKET_BODY   socketBody;
    OS_IPCSOCKET_RETURN socketRet;
    UCHAR buffer[sizeof(socketBody) + ArgsSize];
    UCHAR retvalue = FALSE;
    socketHeader.ArgsTotalSize = ArgsSize;
    socketBody.PID = PID;
    socketBody.Func_num = APInum;      //buzzer API number 1
    socketBody.Func_input_num = ArgsNum;    // Input arguments number
    socketBody.Message_status = 0;    // 0 for finished message
    memmove(buffer, &socketBody, sizeof(socketBody));
    memmove(&buffer[sizeof(socketBody)], Arg,ArgsSize);
    if(write(sockfd, &socketHeader, sizeof(socketHeader)) > 0) 
    if(write(sockfd, buffer, sizeof(socketBody) + ArgsSize) > 0)
    {
        // printf("sendbuffer=");
        // for(int i = 0 ; i < sizeof(socketBody) + ArgsSize ; i++)
        //     printf("%d ", buffer[i]);
        // printf("\n");
        retvalue = TRUE; 
    }
           
    return retvalue;
}

UINT8 IPC_Handler(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 ArgsSize, UINT8 *Arg, UINT8 *retval)
{
    UINT8 recbuffer[1 + ArgsSize];    //API return value(1B)+arguments


    if(IPC_Sender(PID, APInum, ArgsNum, ArgsSize, Arg))
    {
        while(IPC_Reciever(recbuffer, sizeof(recbuffer)) != TRUE);
    
        *retval = recbuffer[0];
        // if(*retval != 0)
        //     printf("PID=%d APInum=%d ret=%d\n", PID, APInum, *retval);
        // memmove(Arg, &recbuffer[1], ArgsSize);
        for(UINT32 i = 0 ; i < ArgsSize ; i++)
        *Arg = recbuffer[1 + i];
        return TRUE;
    }
        
    return FALSE;
}

#define _XOPEN_SOURCE 700

void SegV_handler(int sig)
{
    UINT8* text_STACK_CORRUPT = "[AP] STACK CORRUPTED";


    api_lcdtft_open(0);
    LIB_LCD_Cls();
    write(2, "stack overflow\n", 15);
    LIB_LCD_Puts(0, 0, FONT1+attrREVERSE, strlen(text_STACK_CORRUPT), (UINT8 *)text_STACK_CORRUPT);
    _exit(1);
}

//set shm & enable timer & set signal handler
// #if	0
void IPC_client_Sys_init(void)
{
    return;
    
    static char stack[SIGSTKSZ];
    stack_t ss = {
            .ss_size = SIGSTKSZ,
            .ss_sp = stack,
        };
    struct sigaction sa = {
            .sa_handler = SegV_handler,
            .sa_flags = SA_ONSTACK
        };
        sigaltstack(&ss, 0);
        sigfillset(&sa.sa_mask);
        sigaction(SIGSEGV, &sa, 0);
        // sigaction(SIGABRT, &sa, 0);
    /* fd for the socket */
    
    openSHM();
    // OS_EnableTimer1();  
}
// #endif

int main()
{
    static char stack[SIGSTKSZ];
    stack_t ss = {
        .ss_size = SIGSTKSZ,
        .ss_sp = stack,
    };
    struct sigaction sa = {
        .sa_handler = SegV_handler,
        .sa_flags = SA_ONSTACK
    };
    sigaltstack(&ss, 0);
    sigfillset(&sa.sa_mask);
    sigaction(SIGSEGV, &sa, 0);
    // sigaction(SIGABRT, &sa, 0);
    /* fd for the socket */
  
    openSHM();
  
    //Added by Tammy
    sem_init(sem_A, 1, 0);
    sem_init(sem_B, 1, 0);

    // OS_EnableTimer1();
    APP_main();
}
