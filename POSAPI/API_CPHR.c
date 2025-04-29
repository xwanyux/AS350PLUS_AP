#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
UINT  g_moduls_len = 0;
// ---------------------------------------------------------------------------
// FUNCTION: To encipher plaintext data (pIn) to enciphered data (pOut) by DES.
// INPUT   : pIn  -- the plaintext data. (8-byte)
//           pKey  -- the DES key.       (8-byte)
// OUTPUT  : pOut -- the ciphered data.  (8-byte)
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_des_encipher( UCHAR *pIn, UCHAR *pOut, UCHAR *pKey )
{   
UCHAR retval;
UCHAR sbuf[8+8+8];
  memmove(sbuf,pIn,8);
  memmove(&sbuf[8],pKey,8);
	IPC_clientHandler(psDEV_CPHR,1,3,16,sbuf,&retval);
  memmove(pOut,&sbuf[8+8],8);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To decipher enciphered data (pIn) to plaintext data (pOut) by DES.
// INPUT   : pIn  -- the ciphered data.  (8-byte)
//           pKey -- the DES key.        (8-byte)
// OUTPUT  : pOut -- the plaintext data. (8-byte)
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_des_decipher( UCHAR *pOut, UCHAR *pIn, UCHAR *pKey )
{
UCHAR retval;
UCHAR sbuf[8+8+8];
  memmove(sbuf,pIn,8);
  memmove(&sbuf[8],pKey,8);
	IPC_clientHandler(psDEV_CPHR,2,3,16,sbuf,&retval);
  memmove(pOut,&sbuf[8+8],8);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To encipher plaintext data (pIn) to enciphered data (pOut) by 3DES.
// INPUT   : pIn  -- the plaintext data. (8-byte)
//           pKey -- the 3DES key.       (24-byte)
// OUTPUT  : pOut -- the ciphered data.  (8-byte)
// RETURN  : apiOK
//           apiFailed
// NOTE    : ECB mode. (This function cannot work! To be checked.)
// ---------------------------------------------------------------------------
ULONG	api_3des_encipher( UCHAR *pIn, UCHAR *pOut, UCHAR *pKey )
{
UCHAR retval;
UCHAR sbuf[8+24+8];
  memmove(sbuf,pIn,8);
  memmove(&sbuf[8],pKey,24);
	IPC_clientHandler(psDEV_CPHR,3,3,32,sbuf,&retval);
  memmove(pOut,&sbuf[8+24],8);
	return( retval );
}
// ---------------------------------------------------------------------------
// FUNCTION: To encipher plaintext data (pIn) to enciphered data (pOut) by 3DES.
// INPUT   : pIn  -- the plaintext data. (8 or 8*N -bytes)
//			     inLen - length of the input data
//           pKey -- the 3DES key.       (24-byte)
// OUTPUT  : pOut -- the ciphered data.  (8 or 8*N-byte)
// RETURN  : apiOK
//           apiFailed
// NOTE    : ECB mode. (This function cannot work! To be checked.)
// ---------------------------------------------------------------------------
ULONG	api_3des_encipher2( UCHAR *pIn, UCHAR *pOut, UCHAR *pKey, UCHAR inLen )
{
UCHAR retval;
UCHAR sbuf[1+inLen+24+inLen];
  sbuf[0]=inLen;
  memmove(&sbuf[1],pIn,inLen);
  memmove(&sbuf[1+inLen],pKey,24);
	IPC_clientHandler(psDEV_CPHR,4,4,1+inLen+24,sbuf,&retval);
  memmove(pOut,&sbuf[1+inLen+24],inLen);
	return( retval );

}
// ---------------------------------------------------------------------------
// FUNCTION: To decipher enciphered data (pIn) to plaintext data (pOut) by 3DES.
// INPUT   : pIn  -- the ciphered data.   (8-byte)
//           pKey -- the 3DES key.        (24-byte)
// OUTPUT  : pOut -- the plaintext data.  (8-byte)
// RETURN  : apiOK
//           apiFailed
// NOTE    : ECB mode. (This function cannot work! To be checked.)
// ---------------------------------------------------------------------------
ULONG	api_3des_decipher( UCHAR *pOut, UCHAR *pIn, UCHAR *pKey )
{
UCHAR retval;
UCHAR sbuf[8+24+8];
  memmove(sbuf,pIn,8);
  memmove(&sbuf[8],pKey,24);
	IPC_clientHandler(psDEV_CPHR,5,3,32,sbuf,&retval);
  memmove(pOut,&sbuf[8+24],8);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To decipher enciphered data (pIn) to plaintext data (pOut) by 3DES.
// INPUT   : pIn  -- the plaintext data. (8 or 8*N -bytes)
//			     inLen - length of the input data
//           pKey -- the 3DES key.       (24-byte)
// OUTPUT  : pOut -- the ciphered data.  (8 or 8*N-byte)
// RETURN  : apiOK
//           apiFailed
// NOTE    : ECB mode. (This function cannot work! To be checked.)
// ---------------------------------------------------------------------------
ULONG	api_3des_decipher2( UCHAR *pOut, UCHAR *pIn, UCHAR *pKey, UCHAR inLen )
{
UCHAR retval;
UCHAR sbuf[1+inLen+24+inLen];
  sbuf[0]=inLen;
  memmove(&sbuf[1],pIn,inLen);
  memmove(&sbuf[1+inLen],pKey,24);
	IPC_clientHandler(psDEV_CPHR,6,4,1+inLen+24,sbuf,&retval);
  memmove(pOut,&sbuf[1+inLen+24],inLen);
	return( retval );
}


// ---------------------------------------------------------------------------
// FUNCTION: To encipher plaintext data (pIn) to enciphered data (pOut) by AES.
// INPUT   : pIn     -- the plaintext data. (same length of KeySize)
//           pKey    -- the AES key.	    (key values)
//	     KeySize -- key size.	    (16/24/32 bytes)
// OUTPUT  : pOut    -- the ciphered data.  (same length of KeySize)
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_aes_encipher( UCHAR *pIn, UCHAR *pOut, UCHAR *pKey, UCHAR KeySize )
{
UCHAR retval;
UCHAR sbuf[1+KeySize+KeySize+KeySize];
  sbuf[0]=KeySize;
  memmove(&sbuf[1],pIn,KeySize);
  memmove(&sbuf[1+KeySize],pKey,KeySize);
	IPC_clientHandler(psDEV_CPHR,7,4,1+KeySize+KeySize,sbuf,&retval);
  memmove(pOut,&sbuf[1+KeySize+KeySize],KeySize);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To decipher enciphered data (pIn) to plaintext data (pOut) by AES.
// INPUT   : pIn  -- the ciphered data.     (same length of KeySize)
//           pKey -- the AES key.	    (key values)
//	     KeySize -- key size.	    (16/24/32 bytes)
// OUTPUT  : pOut -- the plaintext data.    (same length of KeySize)
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_aes_decipher( UCHAR *pOut, UCHAR *pIn, UCHAR *pKey, UCHAR KeySize )
{
UCHAR retval;
UCHAR sbuf[1+KeySize+KeySize+KeySize];
  sbuf[0]=KeySize;
  memmove(&sbuf[1],pIn,KeySize);
  memmove(&sbuf[1+KeySize],pKey,KeySize);
	IPC_clientHandler(psDEV_CPHR,8,4,1+KeySize+KeySize,sbuf,&retval);
  memmove(pOut,&sbuf[1+KeySize+KeySize],KeySize);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To encipher plaintext data (pIn) to enciphered data (pOut) by AES(CBC).
// INPUT   : pIn     -- the plaintext data. (same length of KeySize)
//           pKey    -- the AES key.	    (key values)
//	     KeySize -- key size.	    (16/24/32 bytes)
// OUTPUT  : pOut    -- the ciphered data.  (same length of KeySize)
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_aes_cbc_encipher( UCHAR *pIn, UCHAR *pOut, UCHAR *pKey, UCHAR KeySize, UCHAR *iv, UCHAR ivLength)
{
UCHAR retval;
UCHAR sbuf[1+KeySize+KeySize+1+ivLength+KeySize];
  sbuf[0]=KeySize;
  memmove(&sbuf[1],pIn,KeySize);
  memmove(&sbuf[1+KeySize],pKey,KeySize);
  sbuf[1+KeySize+KeySize]=ivLength;
  memmove(&sbuf[1+KeySize+KeySize+1],iv,ivLength);
	IPC_clientHandler(psDEV_CPHR,9,6,1+KeySize+KeySize+1+ivLength,sbuf,&retval);
  memmove(pOut,&sbuf[1+KeySize+KeySize+1+ivLength],KeySize);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To decipher enciphered data (pIn) to plaintext data (pOut) by AES(CBC).
// INPUT   : pIn  -- the ciphered data.     (same length of KeySize)
//           pKey -- the AES key.	    (key values)
//	         KeySize -- key size.	    (16/24/32 bytes)
// OUTPUT  : pOut -- the plaintext data.    (same length of KeySize)
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_aes_cbc_decipher( UCHAR *pIn, UCHAR *pOut, UCHAR *pKey, UCHAR KeySize, UCHAR *iv, UCHAR ivLength)
{
UCHAR retval;
UCHAR sbuf[1+KeySize+KeySize+1+ivLength+KeySize];
  sbuf[0]=KeySize;
  memmove(&sbuf[1],pIn,KeySize);
  memmove(&sbuf[1+KeySize],pKey,KeySize);
  sbuf[1+KeySize+KeySize]=ivLength;
  memmove(&sbuf[1+KeySize+KeySize+1],iv,ivLength);
	IPC_clientHandler(psDEV_CPHR,10,6,1+KeySize+KeySize+1+ivLength,sbuf,&retval);
  memmove(pOut,&sbuf[1+KeySize+KeySize+1+ivLength],KeySize);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To load RSA public key (n,e) for recover function.
// INPUT   : mod -- modulus  (2L-V, max 256 bytes).
//           exp -- exponent (max 3 bytes, value =0x02, 0x03 or 0x010001)
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// NOTE	   : This function is used by EMV L2 kernel only.
// ---------------------------------------------------------------------------
ULONG	api_rsa_loadkey( UCHAR *modulus, UCHAR *exponent )
{
UCHAR retval;
UINT modlen=*modulus+*(modulus+1)*256;
UCHAR sbuf[modlen+2+3];
  g_moduls_len  = modlen;
  memmove(sbuf,modulus,modlen+2);
  memmove(&sbuf[modlen+2],exponent,3);
	IPC_clientHandler(psDEV_CPHR,11,2,modlen+2+3,sbuf,&retval);
	return( retval );
}


// ---------------------------------------------------------------------------
// FUNCTION: To recover the certificate data using the specified RSA public key.
//	     (RSA Public Key Encryption - certificate verification)
// INPUT   : pIn  -- the certificate data. (2L-V)
// OUTPUT  : pOut -- the recovered data.   (2L-V)
// RETURN  : apiOK
//           apiFailed
// NOTE    : pIn & pOut can pointer to the same storage buffer.
// 	     This function is used by EMV L2 kernel only.
// ---------------------------------------------------------------------------
ULONG	api_rsa_recover( UCHAR *pIn, UCHAR *pOut )
{
UCHAR retval;
UINT pInlen=*pIn+*(pIn+1)*256;
// UINT pOutlen=g_moduls_len * ( (pInlen / g_moduls_len) + 1);
UINT pOutlen = pInlen;
UCHAR sbuf[pInlen+2+pOutlen+2];
  memmove(sbuf,pIn,pInlen+2);
	IPC_clientHandler(psDEV_CPHR,12,2,pInlen+2,sbuf,&retval);
  memmove(pOut,&sbuf[pInlen+2],pOutlen+2);
  return( retval );
}


// ---------------------------------------------------------------------------
// FUNCTION: RSA encryption function.
//	     (1) RSA Public Key  (N,e) Encryption - certificate verification.
//	     (2) RSA Private Key (N,d) Decryption - digital signature.
//		 N: modulus
//		 e: public key exponent (generally used: 2, 3, 65537)
//		 d: private key exponent
// INPUT   : cipher	- in API_RSA structure.
// 	     	Mode	- encryption (0) or decryption (1).
//	     	ModLen	- size of modulus   (in bytes).
//	     	Modulus	- RSA key modulus.  (in big-endian)
//	     	ExpLen	- size of exponent  (in bytes )
//	     	Exponent- RSA key exponent. (in big-endian)
//	     	Length	- size of data to be transformed. (in bytes)
//	     	pIn	- data to be transformed.
// OUTPUT  : pOut	- data transformed.
// RETURN  : apiOK
//           apiFailed
// NOTE	   : (1) This is the RSA general purpose function.
//	     (2) Max length of modulus is 2048 bits (256 bytes).
//	     (3) Public key exponent shall be 2, 3 or 65536.
// ---------------------------------------------------------------------------
ULONG	api_rsa_encrypt( API_RSA cipher )
{
ULONG modlen=cipher.ModLen;
ULONG pInlen=cipher.Length;
ULONG explen=cipher.ExpLen;
ULONG pOutlen=(pInlen/modlen)>0?(pInlen-pInlen%modlen+modlen):modlen;
ULONG sendlen=sizeof(API_RSA)+modlen+explen+pInlen;
UCHAR retval;
UCHAR sbuf[sendlen+pOutlen];
  sbuf[0]=cipher.Mode;
  memmove(&sbuf[1],&modlen,sizeof(ULONG));
  memmove(&sbuf[1+sizeof(ULONG)],cipher.Modulus,modlen);
  memmove(&sbuf[1+sizeof(ULONG)+modlen],&explen,sizeof(ULONG));
  memmove(&sbuf[1+sizeof(ULONG)+modlen+sizeof(ULONG)],cipher.Exponent,explen);
  memmove(&sbuf[1+sizeof(ULONG)+modlen+sizeof(ULONG)+explen],&pInlen,sizeof(ULONG));
  memmove(&sbuf[1+sizeof(ULONG)+modlen+sizeof(ULONG)+explen+sizeof(ULONG)],cipher.pIn,pInlen);
  memmove(&sbuf[1+sizeof(ULONG)+modlen+sizeof(ULONG)+explen+sizeof(ULONG)+pInlen],&pOutlen,sizeof(ULONG));
	IPC_clientHandler(psDEV_CPHR,13,1,sendlen,sbuf,&retval);
  memmove(cipher.pOut,&sbuf[sendlen],pOutlen);
  return( retval );
}
// ---------------------------------------------------------------------------
// FUNCTION: To encipher plaintext data (pIn) to enciphered data (pOut) by HMAC-SHA256.
// INPUT   : pIn        -- the plaintext data. 
//           InLength   -- pIn data length in byte
//           pKey       -- the key.    
//           KeyLength  -- The key length in byte
//           OutLength  -- Output data length in byte
// OUTPUT  : pOut       -- the ciphered data.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_HMAC_SHA256_encipher( UCHAR *pIn, UCHAR InLength, UCHAR *pOut, UCHAR OutLength, UCHAR *pKey, UCHAR KeyLength)
{   //argument order:InLength(1B) *pIn(InLength B) KeyLength(1B) *pKey(KeyLength B) OutLength(1B) *pOut(OutLength B)
UCHAR retval;
UINT  sendLen=1+InLength+1+KeyLength+1;
UCHAR sbuf[sendLen+OutLength+10];
// printf("@@@@@@@@@@@@%s %d OutLength=%d sbuf[%d]\n",__func__,__LINE__,OutLength,sizeof(sbuf));
  sbuf[0]=InLength;
  memmove(&sbuf[1],pIn,InLength);
  sbuf[1+InLength]=KeyLength;
  memmove(&sbuf[1+InLength+1],pKey,KeyLength);
  sbuf[1+InLength+1+KeyLength]=OutLength;
	IPC_clientHandler(psDEV_CPHR,14,5,sendLen,sbuf,&retval);
  // printf("@@@@@@@@@@@@%s %d OutLength=%d\n",__func__,__LINE__,OutLength);
  memmove(pOut,&sbuf[sendLen],OutLength);
	return( retval );
}
void	api_rsa_release( void ){return;}