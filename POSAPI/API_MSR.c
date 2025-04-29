


#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
// ---------------------------------------------------------------------------
// FUNCTION: To enable the track(s) of MSR,
//	     all captured data and status will be cleared.
// INPUT   : tracks -- MSR track number.
// OUTPUT  : none.
// RETURN  : DeviceHandleNo
//           apiOutOfService
// ---------------------------------------------------------------------------

UCHAR api_msr_open( UCHAR tracks )
{
UCHAR retval;
	IPC_clientHandler(psDEV_MSR,1,1,1,&tracks,&retval);
	return( retval );
}


// ---------------------------------------------------------------------------
// FUNCTION: To disable all tracks of MSR device.
// INPUT   : dhn
//	     The specified device handle number.
//	     0x00 = to close all opened tasks.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
/*
        close related interrupt
*/
UCHAR api_msr_close( UCHAR dhn )
{
UCHAR retval;
	IPC_clientHandler(psDEV_MSR,2,1,1,&dhn,&retval);
	return( retval );
}




// ---------------------------------------------------------------------------
// FUNCTION: To get the current status of MSR track(s).
// INPUT   : dhn
//	     The specified device handle number.
//	     action
//           0 = clear the status after reading.
//           1 = keep the status after reading.
// OUTPUT  : dbuf
//	     UCHAR  swipe ;            // MSR swiped status.
//	     UCHAR  isoTrk1 ;          // iso track1 status.
//	     UCHAR  isoTrk2 ;          // iso track2 status.
//	     UCHAR  isoTrk3 ;          // iso track3 status.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR api_msr_status( UCHAR dhn, UCHAR action, UCHAR *dbuf )
{
UCHAR retval;
UCHAR args[6];
	args[0]=dhn;
	args[1]=action;
	memmove(&args[2],dbuf,4);
	IPC_clientHandler(psDEV_MSR,3,3,2,args,&retval);
	memmove(dbuf,&args[2],4);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read the captured data from the MSR device.
// INPUT   : dhn
//	     The specified device handle number.
//	     sbuf
//	     UCHAR  action ; 	// 0x00 = system will clear all captured data after reading.
//	                   	// 0x01 = system will keep all captured data after reading.
//	     UCHAR  TrackNo ;	// MSR Track Number
// OUTPUT  : dbuf
//	     struct TRACK  isoTrk1 ;    // contents of iso track1.
//	     struct TRACK  isoTrk2 ;    // contents of iso track2.
//	     struct TRACK  isoTrk3 ;    // contents of iso track3.
//	     struct TRACK {                       // the format of track contents.
//	                  UCHAR  Length ;         // length of the track data.
//	                  UCHAR  StartSentinel ;  // start sentinel of the track.
//	                  UCHAR  Data[Length-2] ; // data in the track.
//                        UCHAR  EndSentinel ;    // end sentinel of the track.
//	                  } ;
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR api_msr_getstring( UCHAR dhn, UCHAR *sbuf, UCHAR *dbuf )
{
UCHAR retval;
UCHAR track1Len;
UCHAR track2Len;
UCHAR track3Len;
UCHAR args[256*3];//one track max length is 256
	args[0]=dhn;
	memmove(&args[1],sbuf,2);
	IPC_clientHandler(psDEV_MSR,4,2,3,args,&retval);
	track1Len=args[3]+1;//length(1B)+sentinel(2B)+data(length-2 B)
	track2Len=args[3+track1Len]+1;//length(1B)+sentinel(2B)+data(length-2 B)
	track3Len=args[3+track1Len+track2Len]+1;//length(1B)+sentinel(2B)+data(length-2 B)
	printf("track1Len=%d track2Len=%d track3Len=%d\n",track1Len,track2Len,track3Len);
	memmove(dbuf,&args[3],track1Len+track2Len+track3Len);
    memset(args, 0x00, sizeof(args));   //clear sensitive data
	return( retval );
}
