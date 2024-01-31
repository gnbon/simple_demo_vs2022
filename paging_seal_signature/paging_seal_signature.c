// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to add, sign and verify paging seal signature
// in PDF document.
//
// NOTE: before using this demo, user should ensure that openssl environment has been prepared:
// a. user should ensure all the openssl header files included by "#include" can be found. (In Windows, User can
//    change project setting "VC++ Directories" -> "Include Directories" or directly change the path
//    used in "#include".)
// b. user should ensure openssl library has been put in the specified path and can be linked to.
//   1. For Windows,
//       Please search "libcrypto.lib" in this file to check the specified path. Or user can directly
//       change the path used to link to "libcrypto.lib". If use dynamic library of openssl, user may
//       need to put dll library of openssl to the folder where ".exe" file is generated before running demo.
//   2. For Linux and Mac, user should put the "libssl.a" and "libcrypto.a"
//      in the directory "../../../lib".

// Include header files.
#include <locale.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#if defined(_WIN32) || defined(_WIN64)
#include<direct.h>
#else
#include <sys/stat.h>
#endif

// Include Foxit SDK header files.
#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_annot_c.h"
#include "../../../include/fs_image_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_signature_c.h"
#include "../../../include/fs_pdfform_c.h"
#include "../../../include/fs_pdfobject_c.h"


// Include openssl header files
#include "../../../include/openssl/rsa.h"
#include "../../../include/openssl/evp.h"
#include "../../../include/openssl/objects.h"
#include "../../../include/openssl/x509.h"
#include "../../../include/openssl/err.h"
#include "../../../include/openssl/pem.h"
#include "../../../include/openssl/ssl.h"
#include "../../../include/openssl/pkcs12.h"
#include "../../../include/openssl/rand.h"
#include "../../../include/openssl/pkcs7.h"
#include "../../../include/openssl/stack.h"
#include "../../../include/openssl/stack.h"
#include "../../../include/openssl/ts.h"
#include "../../../include/openssl/pkcs7.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL (void*)0
#endif

#define MAX_FILE_PATH 260

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

#if defined(_WIN32) || defined(_WIN64)
static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";
#else
static const wchar_t* output_path = L"./output_files/";
static const wchar_t* input_path = L"./input_files/";
#endif

wchar_t output_directory[MAX_FILE_PATH] = { 0 };

int string2wstring(const char *source, size_t source_size, wchar_t *dest, size_t dest_size) {
  const char *_source;
  wchar_t *_dest;
  setlocale(LC_ALL, "chs");
  if (!source || !dest) return 0;
  _source = source;
  _dest = dest;
  wmemset(_dest, 0, dest_size);
  mbstowcs(_dest, _source, dest_size);
  setlocale(LC_ALL, "C");
  return (int)dest_size;
}

char* wstring2string(const wchar_t *source, size_t source_size, char *dest, size_t dest_size) {
  char* curLocale = setlocale(LC_ALL, NULL);
  setlocale(LC_ALL, "chs");
  memset(dest, 0, dest_size);
  wcstombs(dest, source, dest_size);
  setlocale(LC_ALL, "C");
  return dest;
}

void FreeFSBTR(FS_BSTR* wstr) {
  if (!wstr && !wstr->str)
    FSDK_BStr_Clear(wstr);
}

void InitFSBTR(FS_BSTR* bstr, const char* text) {
  FreeFSBTR(bstr);
  FSDK_BStr_Init(bstr);
  FSDK_BStr_SetLength(bstr, (FS_UINT32)strlen(text) + 1);
  FSDK_BStr_Set(bstr, text, (FS_UINT32)strlen(text) + 1);
}

//#if _MSC_VER>=1900
//#include "stdio.h" 
//_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
//#ifdef __cplusplus 
//extern "C"
//#endif 
//FILE* __cdecl __iob_func(unsigned i) {
//    return __acrt_iob_func(i);
//}
//#endif /* _MSC_VER>=1900 */



#if !defined(WIN32) && !defined(_WIN64)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

#if defined(WIN32) || defined(_WIN64)
#include "../../../include/openssl/applink.c"
#pragma  comment(lib,"../../../lib/libcrypto.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment (lib, "crypt32")
#pragma comment(lib, "dbghelp.lib") 
#pragma comment(lib, "shlwapi.lib") 
#pragma comment(lib, "Rpcrt4.lib")
#endif

// Used for implementing SignatureCallback.
typedef struct _DigestContext
{
  SHA_CTX sha_ctx_;
  FSReaderCallback* file_read_callback_;
  FS_UINT32* byte_range_array_;
  FS_UINT32 byte_range_array_size_;
  FS_BSTR digest_;
  FS_BSTR signed_data;
}DigestContext;

typedef struct _SignatureCallbackData
{
  FS_BSTR sub_filter_;
  DigestContext* digest_context_;
  FS_BSTR cert_file_path_;
  FS_BSTR cert_file_password_;
}SignatureCallbackData;

SignatureCallbackData gsignature_callback_data;

void gRelease(void* user_data) {

  FSDK_BStr_Clear(&gsignature_callback_data.sub_filter_);
  FSDK_BStr_Clear(&gsignature_callback_data.cert_file_path_);
  FSDK_BStr_Clear(&gsignature_callback_data.cert_file_password_);
  FSDK_BStr_Clear(&gsignature_callback_data.digest_context_->digest_);
  FSDK_BStr_Clear(&gsignature_callback_data.digest_context_->signed_data);

  free(gsignature_callback_data.digest_context_);
  free(user_data);
}

FS_BOOL gIsNeedPadData(void* user_data) { return FALSE; }
FSCertValidity gCheckCertificateValidity(void* user_data, const wchar_t* cert_path, const wchar_t* cert_password, void* client_data) {
  // User can check the validity of input certificate here.
  // If no need to check, just return e_CertValid.
  return e_FSCertValid;
}


#define FREE_CERT_KEY if(pkey)\
                        EVP_PKEY_free(pkey);\
                      if(x509)\
                        X509_free(x509);\
                      if(ca)\
                        sk_X509_free(ca);\



FS_BOOL GetTextFromFile(const DigestContext* digest_context_, unsigned char* file_buffer) {
  FSReaderCallback* file_read;
  FS_UINT32 offset;
  FS_UINT32 size;
  if (!digest_context_ || !digest_context_->file_read_callback_) return FALSE;
  file_read = digest_context_->file_read_callback_;
  offset = digest_context_->byte_range_array_[0];
  size = digest_context_->byte_range_array_[1];
  file_read->ReadBlock(file_read->user_data, file_buffer, digest_context_->byte_range_array_[0], digest_context_->byte_range_array_[1]);
  file_read->ReadBlock(file_read->user_data, file_buffer + (digest_context_->byte_range_array_[1] - digest_context_->byte_range_array_[0]),
    digest_context_->byte_range_array_[2], digest_context_->byte_range_array_[3]);
  return TRUE;
}

FS_BOOL gStartCalcDigest(void* user_data, const FSReaderCallback* file, const FS_UINT32* byte_range_array,
  FS_UINT32 size_of_array, const FS_SIGNATURE_HANDLE signature, const void* client_data) {
  if (gsignature_callback_data.digest_context_) {
    free(gsignature_callback_data.digest_context_);
    gsignature_callback_data.digest_context_ = NULL;
  }
  gsignature_callback_data.digest_context_ = (DigestContext*)malloc(sizeof(DigestContext));
  FSDK_BStr_Init(&gsignature_callback_data.digest_context_->signed_data);
  FSDK_BStr_Init(&gsignature_callback_data.digest_context_->digest_);
  gsignature_callback_data.digest_context_->file_read_callback_ = (FSReaderCallback*)file;
  gsignature_callback_data.digest_context_->byte_range_array_ = (FS_UINT32*)byte_range_array;
  gsignature_callback_data.digest_context_->byte_range_array_size_ = size_of_array;

  if (!SHA1_Init(&(gsignature_callback_data.digest_context_->sha_ctx_))) {
    free(gsignature_callback_data.digest_context_);
    gsignature_callback_data.digest_context_ = NULL;
    return FALSE;
  }
  return TRUE;
}

FSState gContinueCalcDigest(void* user_data, const void* client_data, const FSPauseCallback* pause) {
  FS_UINT32 file_length;
  unsigned char* file_buffer;
  if (!gsignature_callback_data.digest_context_) return e_FSError;

  file_length = gsignature_callback_data.digest_context_->byte_range_array_[1] + gsignature_callback_data.digest_context_->byte_range_array_[3];
  file_buffer = (unsigned char*)malloc(file_length);
  if (!file_buffer || !GetTextFromFile(gsignature_callback_data.digest_context_, file_buffer)) return e_FSError;

  SHA1_Update(&(gsignature_callback_data.digest_context_->sha_ctx_), file_buffer, file_length);
  free(file_buffer);
  return e_FSFinished;
}

FS_BSTR gGetDigest(void* user_data, const void* client_data) {
  FS_BSTR digest;
  unsigned char* md;
  if (!gsignature_callback_data.digest_context_) {
    digest.str = NULL;
    digest.len = 0;
    return digest;
  };
  md = (unsigned char*)(OPENSSL_malloc((SHA_DIGEST_LENGTH) * sizeof(unsigned char)));
  if (1 != SHA1_Final(md, &(gsignature_callback_data.digest_context_->sha_ctx_))) {
    digest.str = NULL;
    digest.len = 0;
    return digest;
  }

  if (gsignature_callback_data.digest_context_->digest_.str) {
    FSDK_BStr_Clear(&gsignature_callback_data.digest_context_->digest_);
  }

  FSDK_BStr_SetLength(&gsignature_callback_data.digest_context_->digest_, SHA_DIGEST_LENGTH);
  FSDK_BStr_Set(&gsignature_callback_data.digest_context_->digest_, (const char*)md, SHA_DIGEST_LENGTH);

  OPENSSL_free(md);
  return gsignature_callback_data.digest_context_->digest_;
}

unsigned char* PKCS7Sign(const wchar_t* cert_file_path, char* cert_file_password,
  char* plain_text, int plain_text_size, int* signed_data_size);
FS_BOOL PKCS7VerifySignature(FS_BSTR signed_data, FS_BSTR plain_text);

FS_BSTR gSign(void* user_data, const void* digest, FS_UINT32 digest_length, const wchar_t* cert_path,
  const wchar_t* password, FSDigestAlgorithm digest_algorithm,
  void* client_data) {
  FS_BSTR bstr;
  char* plain_text = NULL;
  int signed_data_length = 0;
  size_t password_size;
  size_t pass_word_size;
  char* pass_word = NULL;
  unsigned char* signed_data_buffer;
  bstr.str = NULL;
  bstr.len = 0;
  if (!gsignature_callback_data.digest_context_) {
    return bstr;
  }

  if(strcmp("adbe.pkcs7.sha1", gsignature_callback_data.sub_filter_.str) == 0)  {  
    plain_text = (char*)malloc(digest_length);
    memcpy(plain_text, digest, digest_length);
  }

  password_size = wcslen(password);
  pass_word_size = password_size + 1;
  pass_word = (char*)malloc(pass_word_size);
  signed_data_buffer = PKCS7Sign(cert_path, wstring2string(password, password_size, pass_word, pass_word_size),
    plain_text, digest_length, &signed_data_length);
  free(pass_word);
  free(plain_text);
  if (!signed_data_buffer) return bstr;
  if (!gsignature_callback_data.digest_context_->signed_data.str) {
    FSDK_BStr_Clear(&gsignature_callback_data.digest_context_->signed_data);
  }
  
  FSDK_BStr_SetLength(&gsignature_callback_data.digest_context_->signed_data, signed_data_length);
  FSDK_BStr_Set(&gsignature_callback_data.digest_context_->signed_data, (char*)(const char*)signed_data_buffer, signed_data_length);
  free(signed_data_buffer);
  return gsignature_callback_data.digest_context_->signed_data;
}

FS_BSTR gSign0(void* user_data, const void* digest, FS_UINT32 digest_length, FSIFX_FileStream* cert_file_stream, const wchar_t* password, FSDigestAlgorithm digest_algorithm, void* client_data) {
  FS_BSTR bstr;
  bstr.str = "";
  bstr.len = 0;
  return bstr;
}

FS_UINT32 gVerifySigState(void* user_data, const void* digest, FS_UINT32 digest_length, const void* signed_data, FS_UINT32 signed_data_len, void* client_data) {
  // Usually, the content of a signature field is contain the certification of signer.
  // But we can't judge this certification is trusted.
  // For this example, the signer is ourself. So when using api PKCS7_verify to verify,
  // we pass NULL to it's parameter <i>certs</i>.
  // Meanwhile, if application should specify the certificates, we suggest pass flag PKCS7_NOINTERN to
  // api PKCS7_verify.
  FS_BSTR plain_text;
  unsigned char* file_buffer = NULL;
  FS_BSTR signed_data_str;
  FS_BOOL ret;
  if (!gsignature_callback_data.digest_context_) return e_FSStateVerifyErrorData;
  if (strcmp("adbe.pkcs7.sha1", gsignature_callback_data.sub_filter_.str) == 0) {
    FSDK_BStr_Init(&plain_text);
    FSDK_BStr_SetLength(&plain_text, digest_length);
    FSDK_BStr_Set(&plain_text, digest, digest_length);
  } else {
    return e_FSStatesStateUnknown;
  }  
  FSDK_BStr_Init(&signed_data_str);
  FSDK_BStr_SetLength(&signed_data_str, signed_data_len);
  FSDK_BStr_Set(&signed_data_str, signed_data, signed_data_len);

  ret = PKCS7VerifySignature(signed_data_str, plain_text);
  if (file_buffer) free(file_buffer);
  FSDK_BStr_Clear(&signed_data_str);
  FSDK_BStr_Clear(&plain_text);
  //this function is only used to verify the intergrity of a signature.
  return ret ? e_FSStateVerifyNoChange : e_FSStateVerifyChange;
}

ASN1_INTEGER* CreateNonce(int bits) {
  unsigned char buf[20];
  int i = 0;
  ASN1_INTEGER* nonce = NULL;
  int len = (bits - 1) / 8 + 1;
  // Generating random byte sequence.
  if (len > (int)sizeof(buf)) {
    return NULL;
  }
  if (RAND_bytes(buf, len) <= 0) {
    return NULL;
  }
  // Find the first non-zero byte and creating ASN1_INTEGER object.
  for (i = 0; i < len && !buf[i]; ++i);
  if (!(nonce = ASN1_INTEGER_new())) {
    ASN1_INTEGER_free(nonce);
    return NULL;
  }
  OPENSSL_free(nonce->data);
  // Allocate at least one byte.
  nonce->length = len - i;
  if (!(nonce->data = (unsigned char*)(OPENSSL_malloc(nonce->length + 1)))) {
    ASN1_INTEGER_free(nonce);
    return NULL;
  }
  memcpy(nonce->data, buf + i, nonce->length);
  return nonce;
}

FS_BOOL ParseP12File(const wchar_t* cert_file_path, char* cert_file_password,
  EVP_PKEY** pkey, X509** x509, STACK_OF(X509)** ca) {
  FILE* file = NULL;
  PKCS12* pkcs12;
#if defined(_WIN32) || defined(_WIN64)
  _wfopen_s(&file, cert_file_path, L"rb");
#else
  file = fopen(string::FromUnicode(cert_file_path), "rb");
#endif  // defined(_WIN32) || defined(_WIN64)
  if (!file) {
    return FALSE;
  }

  pkcs12 = d2i_PKCS12_fp(file, NULL);
  fclose(file);
  if (!pkcs12) {
    return FALSE;
  }

  if (!PKCS12_parse(pkcs12, cert_file_password, pkey, x509, ca)) {
    return TRUE;
  }

  PKCS12_free(pkcs12);
  if (!pkey)
    return FALSE;
  return TRUE;
}

#define HANDLE_CREATE_TS_ERROR 	{\
									if (!ret)  {\
										TS_REQ_free(ts_req);\
										ts_req = NULL;\
										printf("could not create query\n");\
									}\
									TS_MSG_IMPRINT_free(msg_imprint);\
									X509_ALGOR_free(algo);\
									ASN1_OBJECT_free(policy_obj);\
									ASN1_INTEGER_free(nonce_asn1);\
									return ts_req;\
								}

ASN1_INTEGER *create_nonce(int bits)
{
  unsigned char buf[20];
  ASN1_INTEGER *nonce = NULL;
  int len = (bits - 1) / 8 + 1;
  int i;

  /* Generating random byte sequence. */
  if (len > (int)sizeof(buf))
  {
    printf("bit count error\n");
    ASN1_INTEGER_free(nonce);
    return NULL;
  }
  if (RAND_bytes(buf, len) <= 0)
  {
    printf("can not generate random number\n");
    ASN1_INTEGER_free(nonce);
    return NULL;
  }

  /* Find the first non-zero byte and creating ASN1_INTEGER object. */
  for (i = 0; i < len && !buf[i]; ++i);
  if (!(nonce = ASN1_INTEGER_new()))
  {
    printf("could not create nonce\n");
    ASN1_INTEGER_free(nonce);
    return NULL;
  }

  OPENSSL_free(nonce->data);
  /* Allocate at least one byte. */
  nonce->length = len - i;
  if (!(nonce->data = (unsigned char *)OPENSSL_malloc(nonce->length + 1)))
  {
    printf("out of memory\n");
    ASN1_INTEGER_free(nonce);
    return NULL;
  }

  memcpy(nonce->data, buf + i, nonce->length);

  return nonce;
}

TS_REQ *create_ts_query(unsigned char *digest, int len)
{
  int ret = 0;
  TS_REQ *ts_req = NULL;
  const EVP_MD *md;
  TS_MSG_IMPRINT *msg_imprint = NULL;
  X509_ALGOR *algo = NULL;
  ASN1_OBJECT *policy_obj = NULL;
  ASN1_INTEGER *nonce_asn1 = NULL;

  switch (len) {
  case 20:
    md = EVP_get_digestbyname("sha1");
    break;
  case 32:
    md = EVP_get_digestbyname("sha256");
    break;
  default:
    HANDLE_CREATE_TS_ERROR;
  }

  /* Creating request object. */
  if (!(ts_req = TS_REQ_new()))
    HANDLE_CREATE_TS_ERROR;

  /* Setting version. */
  if (!TS_REQ_set_version(ts_req, 1))
    HANDLE_CREATE_TS_ERROR;

  /* Creating and adding MSG_IMPRINT object. */
  if (!(msg_imprint = TS_MSG_IMPRINT_new()))
    HANDLE_CREATE_TS_ERROR;

  /* Adding algorithm. */
  if (!(algo = X509_ALGOR_new()))
    HANDLE_CREATE_TS_ERROR;
  if (!(algo->algorithm = OBJ_nid2obj(EVP_MD_type(md))))
    HANDLE_CREATE_TS_ERROR;
  if (!(algo->parameter = ASN1_TYPE_new()))
    HANDLE_CREATE_TS_ERROR;
  algo->parameter->type = V_ASN1_NULL;
  if (!TS_MSG_IMPRINT_set_algo(msg_imprint, algo))
    HANDLE_CREATE_TS_ERROR;

  if (!TS_MSG_IMPRINT_set_msg(msg_imprint, digest, len))
    HANDLE_CREATE_TS_ERROR;

  if (!TS_REQ_set_msg_imprint(ts_req, msg_imprint))
    HANDLE_CREATE_TS_ERROR;

  /* Setting nonce if requested. */
  if (!(nonce_asn1 = create_nonce(64)))
    HANDLE_CREATE_TS_ERROR;
  if (nonce_asn1 && !TS_REQ_set_nonce(ts_req, nonce_asn1))
    HANDLE_CREATE_TS_ERROR;

  /* Setting certificate request flag if requested. */
  if (!TS_REQ_set_cert_req(ts_req, 1))
    HANDLE_CREATE_TS_ERROR;

  ret = 1;
  HANDLE_CREATE_TS_ERROR;
}


long Get_TS_REP(unsigned char *md, int md_len, unsigned char **pRet)
{
  unsigned char *p;
  unsigned char *tsReq;
  int len;
  char SendBuffer[1024] = { 0 };
  const char* bsAuth = ":";
  FS_BSTR bsDestAuth;
  WSADATA  Ws;
  SOCKET CientSocket;
  unsigned char *totalBuf;
  struct sockaddr_in ServerAddr;
  int Ret = 0;
  int AddrLen = 0;
  HOSTENT * iphost;
  char serverIP[20] = "";
  struct servent *pST;
  int sendLen;
  char recvBuf[8096] = { 0 };
  int revtotalLen = 0, recTSL = 0;
  int revLen;
  int nTSRepPos = 0;
  int tsrepL = 0;
  char rspCode[4] = { 0 };
  char *ptr = NULL;

	TS_REQ *request = create_ts_query(md, md_len);
	len = i2d_TS_REQ(request, NULL);
	tsReq = (unsigned char *)OPENSSL_malloc(len);
	p = tsReq;
	len = i2d_TS_REQ(request, (unsigned char **)&p);
	TS_REQ_free(request);

	FSDK_Codec_Base64Encode(bsAuth, 1*sizeof(char), &bsDestAuth);

  sprintf_s(SendBuffer, 1024, "GET %s HTTP/1.1\r\n"
    "Accept: */*\r\n"
    "Content-Type: application/timestamp-query\r\n"
    "Content-Length: %d\r\n"
    "Character-Encoding: binary\r\n"
    "User-Agent: PPKHandler\r\n"
    "Host: %s\r\n"
    "Connection: Keep-Alive\r\n"
    "Cache-Control: no-cache\r\n"
    "Authorization: Basic %s\r\n"
    "\r\n",
    "/TSAServer.aspx", len, "ca.signfiles.com", bsDestAuth.str);

 
	totalBuf = (unsigned char *)malloc(len + strlen(SendBuffer));

	memcpy(totalBuf, SendBuffer, strlen(SendBuffer));
	memcpy(totalBuf + strlen(SendBuffer), tsReq, len);
	OPENSSL_free(tsReq);
	WSAStartup(MAKEWORD(2, 2), &Ws);
	CientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ((iphost = gethostbyname("ca.signfiles.com")) != NULL)
	{
		int i = 0;
		while (iphost->h_addr_list[i])
		{
			memcpy(&serverIP, inet_ntoa(*((struct in_addr *)iphost->h_addr_list[i])), 20);
			i++;
		}
	}

	pST = getservbyname("http", NULL);
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(serverIP);
	ServerAddr.sin_port = pST->s_port;
	memset(ServerAddr.sin_zero, 0x00, 8);
	connect(CientSocket, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	sendLen = send(CientSocket, (const char *)totalBuf, len + (int)strlen(SendBuffer), 0);

	free(totalBuf);
	revLen = recv(CientSocket, recvBuf, 8096, 0);
	if (revLen == 0)
	{
		printf("\r\ntsa server unreachable.\r\n");
		closesocket(CientSocket);
		WSACleanup();
		return 0;
	}
	revtotalLen += revLen;

	//get the ts reponse length
  strncpy_s(rspCode, 4, recvBuf + 9, 3);
	
	if (strcmp(rspCode, "200") != 0)
	{
		printf("\r\n tsa server refuse request.\r\n%s\r\n", recvBuf);
		closesocket(CientSocket);
		WSACleanup();
		return 0;
	}

  ptr = strstr(recvBuf, "Content-Length") + strlen("Content-Length") + 2;
	//nTSRepPos = (int)(strRecv.find("Content-Length") + strlen("Content-Length") + 2);

	while (*ptr > 47 && *ptr < 58)
	{
		tsrepL = tsrepL * 10 + *ptr - 48;
    ptr += 1;
	}

	while (recTSL < tsrepL && revtotalLen < tsrepL)
	{
		printf("retry, len:%d\n", revLen);
		revLen = recv(CientSocket, recvBuf + revtotalLen, 8096 - revtotalLen, 0);
		recTSL += revLen;
		revtotalLen += revLen;
	}
	*pRet = (unsigned char *)OPENSSL_malloc(tsrepL);
	memcpy(*pRet, recvBuf + revtotalLen - tsrepL, tsrepL);

	closesocket(CientSocket);
	WSACleanup();

	return tsrepL;
}

int append_tsp_token(PKCS7_SIGNER_INFO *sinfo, unsigned char *ts_rep, long tsrepL)
{

	unsigned char *p = ts_rep;
	TS_RESP *tsp = d2i_TS_RESP(NULL, (const unsigned char**)&p, tsrepL);
	if (tsp != NULL)
	{
    int p7_len;
    unsigned char *p7_der = NULL;
    unsigned char *p = NULL;
    PKCS7* token = TS_RESP_get_token(tsp);
		if (!PKCS7_type_is_signed(token))
		{
			printf("Error in timestamp token: not signed!\n");
			return 1;
		}

		p7_len = i2d_PKCS7(token, NULL);
		p7_der = (unsigned char *)OPENSSL_malloc(p7_len);
		p = p7_der;
		i2d_PKCS7(token, &p);

		if (sinfo)
		{
			//Add timestamp token to the PKCS7 signature object
			ASN1_STRING *value = ASN1_STRING_new();
			ASN1_STRING_set(value, p7_der, p7_len);
			PKCS7_add_attribute(sinfo, NID_id_smime_aa_timeStampToken, V_ASN1_SEQUENCE, value);
			OPENSSL_free(p7_der);
		}
	} else {
		printf("Error decoding timestamp token!\n");
		return 1;
	}
	return 0;
}

unsigned char* PKCS7Sign(const wchar_t* cert_file_path, char* cert_file_password,
  char* plain_text, int plain_text_size, int* signed_data_size) {
  PKCS7* p7 = NULL;
  EVP_PKEY* pkey = NULL;
  X509* x509 = NULL;
  BIO* p7bio = NULL;
  STACK_OF(X509)* ca = NULL;
  unsigned char md256[32] = "";
  unsigned char *pTSRep = NULL;
  long lTSRepLength;
  int i;
  PKCS7_SIGNER_INFO* signer_info;
  unsigned long der_length;
  unsigned char* der;
  unsigned char* der_temp;
  if (!ParseP12File(cert_file_path, cert_file_password, &pkey, &x509, &ca))
    return NULL;

  p7 = PKCS7_new();
  PKCS7_set_type(p7, NID_pkcs7_signed);
  PKCS7_content_new(p7, NID_pkcs7_data);

  // Application should not judge the sign algorithm with the content's length.
  // Here, just for convenient;
  if (plain_text_size > 32)
    PKCS7_ctrl(p7, PKCS7_OP_SET_DETACHED_SIGNATURE, 1, NULL);

  signer_info = PKCS7_add_signature(p7, x509, pkey, EVP_sha1());
  PKCS7_add_certificate(p7, x509);

  for(i = 0; i < OPENSSL_sk_num((const OPENSSL_STACK*)ca); i++)
    PKCS7_add_certificate(p7, (X509*)OPENSSL_sk_value((const OPENSSL_STACK*)ca, i));
  // Set source data to BIO.
  p7bio = PKCS7_dataInit(p7, NULL);
  BIO_write(p7bio, plain_text, plain_text_size);
  PKCS7_dataFinal(p7, p7bio);

   //append the time stamp token to pkcs7 structure.
  SHA256(signer_info->enc_digest->data, signer_info->enc_digest->length, md256);

  lTSRepLength = Get_TS_REP(md256, 32, &pTSRep);
  if (lTSRepLength != 0)
	  append_tsp_token(signer_info, pTSRep, lTSRepLength);
  OPENSSL_free(pTSRep);

  FREE_CERT_KEY;
  BIO_free_all(p7bio);
  // Get signed data.
  der_length = i2d_PKCS7(p7, NULL);
  der = (unsigned char*)(malloc(der_length));
  memset(der, 0, der_length);
  der_temp = der;
  i2d_PKCS7(p7, &der_temp);
  PKCS7_free(p7);
  *signed_data_size = (int)der_length;
  return (unsigned char*)der;
}

FS_BOOL PKCS7VerifySignature(FS_BSTR signed_data, FS_BSTR plain_text) {
  int i;
  PKCS7* p7;
  int sign_count;
  int length = 0;
  FS_BOOL bSigAppr = FALSE;
  unsigned char *p = NULL;
  STACK_OF(PKCS7_SIGNER_INFO) *sk;
  BIO* vin;
  // Retain PKCS7 object from signed data.
  vin = BIO_new_mem_buf((void*)signed_data.str, signed_data.len);
  p7 = d2i_PKCS7_bio(vin, NULL);
  sk = PKCS7_get_signer_info(p7);
  sign_count = sk_PKCS7_SIGNER_INFO_num(sk);

  for (i = 0; i < sign_count; i++) {
    BIO *p7bio;
    X509 *x509;
    PKCS7_SIGNER_INFO* sign_info = sk_PKCS7_SIGNER_INFO_value(sk, i);
    p7bio = BIO_new_mem_buf((void*)plain_text.str, plain_text.len);
    x509 = PKCS7_cert_from_signer_info(p7, sign_info);
    if (1 == PKCS7_verify(p7, NULL, NULL, p7bio, NULL, PKCS7_NOVERIFY))
      bSigAppr = TRUE;
    BIO_free(p7bio);
  }
  PKCS7_free(p7);
  BIO_free(vin);
  return bSigAppr;
}

////////////////////////
char* DateTimeToString(char* datetime_str, int datetime_size, const FSDateTime* datetime) {
  if (!datetime_str || !datetime)  return NULL;
  sprintf_s(datetime_str, datetime_size, "%d/%d/%d-%d:%d:%d %s%d:%d", datetime->year, datetime->month, datetime->day, datetime->hour,
    datetime->minute, datetime->second, datetime->utc_hour_offset > 0 ? "+" : "-", datetime->utc_hour_offset,
    datetime->utc_minute_offset);
  return datetime_str;
}

FSDateTime GetLocalDateTime() {
  struct tm* rime;
  long time_zone = 0;
  int timezone_hour;
  int timezone_minute;
  FSDateTime datetime;
  time_t t = time(NULL);
  struct tm _Tm;
  localtime_s(&_Tm, &t);
  rime = &_Tm;
  _tzset();

  _get_timezone(&time_zone);
  timezone_hour = time_zone / 3600 * -1;
  timezone_minute = (abs(time_zone) % 3600) / 60;

  datetime.year = (FS_UINT16)(rime->tm_year + 1900);
  datetime.month = (FS_UINT16)(rime->tm_mon + 1);
  datetime.day = (FS_UINT16)(rime->tm_mday);
  datetime.hour = (FS_UINT16)(rime->tm_hour);
  datetime.minute = (FS_UINT16)(rime->tm_min);
  datetime.second = (FS_UINT16)(rime->tm_sec);
  datetime.milliseconds = 0;
  datetime.utc_hour_offset = timezone_hour;
  datetime.utc_minute_offset = timezone_minute;

  return datetime;
}

char* TransformSignatureStateToString(char* str_state, size_t state_size, FS_UINT32 sig_state) {
  
  char* state_ptr = str_state;
  size_t size = state_size;

  int state[] = { e_FSStatesStateUnknown, e_FSStateNoSignData, e_FSStateUnsigned, e_FSStateSigned, e_FSStateVerifyValid,
                      e_FSStateVerifyInvalid, e_FSStateVerifyErrorData, e_FSStateVerifyNoSupportWay, e_FSStateVerifyErrorByteRange,
                      e_FSStateVerifyChange, e_FSStateVerifyIncredible, e_FSStateVerifyNoChange, e_FSStateVerifyIssueValid,
                      e_FSStatesStateVerifyIssueUnknown, e_FSStateVerifyIssueRevoke, e_FSStateVerifyIssueExpire, e_FSStateVerifyIssueUncheck
                      , e_FSStateVerifyIssueCurrent, e_FSStateVerifyTimestampNone, e_FSStateVerifyTimestampDoc, e_FSStateVerifyTimestampValid,
                      e_FSStateVerifyTimestampInvalid, e_FSStateVerifyTimestampExpire, e_FSStatesStateVerifyTimestampIssueUnknown,
                      e_FSStateVerifyTimestampIssueValid, e_FSStateVerifyTimestampTimeBefore, e_FSStateCertCannotGetVRI };

  const char* state_str[100] = { "Unknown", "NoSignData", "Unsigned", "Signed", "VerfiyValid",
                    "VerifyInvalid", "VerifyErrorData", "VerifyNoSupportWay", "VerifyErrorByteRange",
                    "VerifyChange", "VerifyIncredible", "VerifyNoChange", "VerifyIssueValid",
                    "VerifyIssueUnknown", "VerifyIssueRevoke", "VerifyIssueExpire", "VerifyIssueUncheck",
                    "VerifyIssueCurrent", "VerifyTimestampNone", "VerifyTimestampDoc", "VerifyTimestampValid",
                    "VerifyTimestampInvalid", "VerifyTimestampExpire", "VerifyTimestampIssueUnknown",
                    "VerifyTimestampIssueValid", "VerifyTimestampTimeBefore", "CertCannotGetVRI" };
  int num = sizeof(state) / sizeof(state[0]);
  int i;
  if (!str_state) return NULL;
  for (i = 0; i <num; i++) {
    if (sig_state & state[i]){
      if (size != state_size) {
        strcpy_s(state_ptr, size, "|");
        state_ptr++;
        size -= 1;
      }
      strcpy_s(state_ptr, size, state_str[i]);
      size -= strlen(state_str[i]);
      state_ptr += strlen(state_str[i]);
    }
  }

  return str_state;
}

FS_SIGNATURE_HANDLE AddPagingSealSiganture(FS_PDFDOC_HANDLE pdf_doc, const char* sub_filter, FS_BOOL is_on_perforation) {
  FS_PDFPAGE_HANDLE pdf_page;
  float page_width;
  float page_height;
  int page_count = 0;
  float sig_width;
  float sig_hegiht;
  FSPagingSealConfig paging_seal_config;
  FS_RANGE_HANDLE range = NULL;
  FS_PAGINGSEALSIGNATURE_HANDLE new_sig = NULL;
  FS_BOOL return_IsEmpty;
  FS_BOOL return_value;
  FSDateTime sign_time;
  char new_value[100] = { 0 };
  wchar_t w_new_value[200] = { 0 };
  wchar_t  image_file_path[MAX_FILE_PATH] = { 0 };
  FS_IMAGE_HANDLE image;
  FS_UINT32 ap_flags;
  FSErrorCode code = FSDK_PDFDoc_GetPage(pdf_doc, 0, &pdf_page);
  
  FSDK_PDFPage_GetWidth(pdf_page, &page_width);  
  FSDK_PDFPage_GetHeight(pdf_page, &page_height);  
  FSDK_PDFDoc_GetPageCount(pdf_doc, &page_count);  
  FSDK_Range_Create1(1, page_count, e_FSAll, &range);
  sig_width = (float)page_width * 0.4;
  sig_hegiht = (float)page_height * 0.1;
  // Add a new signature to page.

  FSDK_PDFDoc_AddPagingSealSignature(pdf_doc, range, sig_width, sig_hegiht, &new_sig);
  FSDK_Signature_IsEmpty(new_sig, &return_IsEmpty);
  if (return_IsEmpty) return new_sig;

  paging_seal_config.first_page_percent = 0.5f;
  paging_seal_config.paging_seal_position = e_FSPagingSealPositionLeft;
  paging_seal_config.offset = (float)page_width / 2;
  paging_seal_config.is_on_perforation = is_on_perforation;
  paging_seal_config.paging_seal_style = e_FSPagingSealStyleMultipleSignatures;
  FSDK_PagingSealSignature_SetPagingSealConfig(new_sig, paging_seal_config);
  FSDK_PagingSealSignature_GenerateAppearance(new_sig, &return_value);

  // Set values for the new signature.
  FSDK_Signature_SetKeyValue(new_sig,e_FSKeyNameSigner, L"Foxit PDF SDK");
  sprintf_s(new_value,100, "As a sample for subfilter \"%s\"", sub_filter);
  string2wstring(new_value, strlen(new_value), w_new_value, 200);
  FSDK_Signature_SetKeyValue(new_sig,e_FSKeyNameReason, w_new_value);
  FSDK_Signature_SetKeyValue(new_sig,e_FSKeyNameContactInfo, L"support@foxitsoftware.com");
  FSDK_Signature_SetKeyValue(new_sig, e_FSKeyNameDN, L"CN=CN,MAIL=MAIL@MAIL.COM");
  FSDK_Signature_SetKeyValue(new_sig, e_FSKeyNameLocation, L"Fuzhou, China");
  FSDK_Signature_SetKeyValue(new_sig, e_FSKeyNameText, w_new_value);
  sign_time = GetLocalDateTime();
  FSDK_Signature_SetSignTime(new_sig, sign_time);

  swprintf_s(image_file_path, MAX_FILE_PATH, L"%sFoxitLogo.jpg", input_path);  
  FSDK_Image_Create0(image_file_path, &image);
  FSDK_Signature_SetImage(new_sig, image, 0);
  // Set appearance flags to decide which content would be used in appearance.
  ap_flags = e_FSAPFlagLabel | e_FSAPFlagSigner |
    e_FSAPFlagReason | e_FSAPFlagDN |
    e_FSAPFlagLocation | e_FSAPFlagText |
    e_FSAPFlagSigningTime | e_FSAPFlagBitmap;
  FSDK_Signature_SetAppearanceFlags(new_sig, ap_flags);
  FSDK_PagingSealSignature_GenerateAppearance(new_sig, &return_value);
  return new_sig;
}

void AdobePPKLitePagingSealSignature(FS_PDFDOC_HANDLE pdf_doc, FS_BOOL use_default) {
  const char* filter = "Adobe.PPKLite";
  const char* sub_filter = "adbe.pkcs7.detached";

  FS_PDFPAGE_HANDLE pdf_page = NULL;
  FS_PAGINGSEALSIGNATURE_HANDLE new_signature = NULL;
  FS_BOOL is_signed;
  char sig_state_str[1000] = { 0 };
  size_t sig_state_size = 1000;
  FS_UINT32 sig_state;
  wchar_t signed_pdf_path[MAX_FILE_PATH] = { 0 };
  wchar_t cert_file_path[MAX_FILE_PATH] = { 0 };
  wchar_t *cert_file_password = L"123456";
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  FS_PDFDOC_HANDLE signed_pdf_doc;
  int sig_count = 0;
  FS_SIGNATURE_HANDLE signed_signature = NULL;
  FS_PROGRESSIVE_HANDLE progressive2 = NULL;
  FSErrorCode code;

  if (!use_default) { 
    FS_BOOL return_Callback;
    FSSignatureCallback* sig_callback = (FSSignatureCallback*)malloc(sizeof(FSSignatureCallback));
    sig_callback->user_data = sig_callback;
    sig_callback->StartCalcDigest = gStartCalcDigest;
    sig_callback->ContinueCalcDigest = gContinueCalcDigest;
    sig_callback->GetDigest = gGetDigest;
    sig_callback->Sign = gSign;
    sig_callback->Sign0 = gSign0;
    sig_callback->VerifySigState = gVerifySigState;
    sig_callback->IsNeedPadData = gIsNeedPadData;
    sig_callback->CheckCertificateValidity = gCheckCertificateValidity;
    InitFSBTR(&gsignature_callback_data.sub_filter_, "adbe.pkcs7.sha1");
    FSDK_BStr_Init(&gsignature_callback_data.cert_file_path_);
    FSDK_BStr_Init(&gsignature_callback_data.cert_file_password_);
    gsignature_callback_data.digest_context_ = NULL;
    FSDK_Library_RegisterSignatureCallback(filter, gsignature_callback_data.sub_filter_.str, sig_callback, &return_Callback);
  }

  if (!use_default) {
    printf("Use signature callback object for filter \"%s\" and sub-filter \"%s\"\r\n",
      filter, gsignature_callback_data.sub_filter_.str);
  } else {
    printf("Use signature callback object for filter \"%s\" and sub-filter \"%s\"\r\n",
      filter, sub_filter);
  }

  // Add a new signature to first page.
  if (!use_default) {
    new_signature = AddPagingSealSiganture(pdf_doc, gsignature_callback_data.sub_filter_.str, TRUE);
    FSDK_Signature_SetSubFilter(new_signature, gsignature_callback_data.sub_filter_.str);
  } else {
    new_signature = AddPagingSealSiganture(pdf_doc, sub_filter, FALSE);
    FSDK_Signature_SetSubFilter(new_signature, sub_filter);
  }
   
  FSDK_Signature_IsSigned(new_signature, &is_signed);  
  FSDK_Signature_GetState(new_signature, &sig_state);
  printf("[Before signing] Signed?:%s\t State:%s\r\n",
    is_signed ? "true" : "false",
    TransformSignatureStateToString(sig_state_str, sig_state_size, sig_state));

  // Sign the new signature.
  swprintf_s(signed_pdf_path, MAX_FILE_PATH, L"%ssigned_middle_paging_seal_signature.pdf", output_directory);

  if (use_default)
    swprintf_s(signed_pdf_path, MAX_FILE_PATH, L"%ssigned_left_paging_seal_signature_default_handler.pdf", output_directory);  
  swprintf_s(cert_file_path, MAX_FILE_PATH, L"%sfoxit_all.pfx", input_path);  

  // Cert file path will be passed back to application through callback function FSSignatureCallback::Sign().
  // In this demo, the cert file path will be used for signing in callback function FSSignatureCallback::Sign().  
  FSDK_Signature_StartSign(new_signature, cert_file_path, cert_file_password, e_FSDigestSHA1, signed_pdf_path, NULL, NULL, &progressive);
  printf("[Sign] Finished!\r\n");
  FSDK_Signature_IsSigned(new_signature, &is_signed);
  FSDK_Signature_GetState(new_signature, &sig_state);
  memset(sig_state_str, 0, sig_state_size);
  printf("[After signing] Signed?:%s\tState:%s\r\n",
    is_signed ? "true" : "false",
    TransformSignatureStateToString(sig_state_str, sig_state_size, sig_state));

  // Open the signed document and verify the newly added signature (which is the last one).
  wprintf(L"Signed PDF file: %s\r\n",signed_pdf_path);  
  code = FSDK_PDFDoc_Create0(signed_pdf_path, &signed_pdf_doc);
  if (code != e_FSErrSuccess) return;
  code = FSDK_PDFDoc_Load(signed_pdf_doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(signed_pdf_doc);
    wprintf(L"The Doc [%s] Error: %d\n",signed_pdf_path, code);
    return;
  }
  // Get the last signature which is just added and signed. 
  FSDK_PDFDoc_GetSignatureCount(signed_pdf_doc, &sig_count);    
  FSDK_PDFDoc_GetSignature(signed_pdf_doc, sig_count - 1, &signed_signature);
  // Verify the intergrity of signature.
  FSDK_Signature_StartVerify(signed_signature,NULL ,NULL, &progressive2);
  printf("[Verify] Finished!\r\n");
  FSDK_Signature_IsSigned(signed_signature, &is_signed);
  FSDK_Signature_GetState(signed_signature, &sig_state);
  memset(sig_state_str, 0, sig_state_size);
  printf("[After verifying] Signed?:%s\tState:%s\r\n",
    is_signed ? "true" : "false",
    TransformSignatureStateToString(sig_state_str, sig_state_size, sig_state));
}

void CheckPagingSealSignatureInfo(FS_PDFDOC_HANDLE pdf_doc) {
  int sig_count;
  int i;
  FSDK_PDFDoc_GetSignatureCount(pdf_doc, &sig_count);
  if (sig_count < 1) {
    printf("No signature in current PDF file.\r\n");
    return;
  }
  for (i = 0; i < sig_count; i++) {
    FS_SIGNATURE_HANDLE signature = NULL;
    FS_WIDGETANNOT_HANDLE widget_annot = NULL;
    FS_CONTROL_HANDLE control_handle = NULL;
    FS_PDFDICTIONARY_HANDLE return_value;
    FS_UINT32 return_value2;
    FS_BSTR filter_new;
    FS_BSTR sub_filter_new;
    FS_WSTR key_value_new;
    FS_BOOL return_IsEmpty;
    FS_BOOL is_signed;
    FS_UINT32 sig_state;
    char sig_state_str[256] = { 0 };
    printf("Signature index: %d\r\n", i);
    FSDK_PDFDoc_GetSignature(pdf_doc, i, &signature);
    FSDK_Signature_IsEmpty(signature, &return_IsEmpty);
    if (return_IsEmpty) continue;
    FSDK_Signature_IsSigned(signature, &is_signed);
    printf("Signed?:%s\r\n", is_signed ? "true" : "false");
    FSDK_Signature_GetState(signature, &sig_state);
    printf("State:%s\r\n", TransformSignatureStateToString(sig_state_str, 256, sig_state));    
    FSDK_Field_GetControl(signature, 0, &control_handle);
    FSDK_Control_GetWidget(control_handle, &widget_annot);    
    FSDK_Annot_GetDict(widget_annot, &return_value);    
    FSDK_PDFObject_GetObjNum(return_value, &return_value2);
    printf("Object number %u of related widget annotation's dictionary\r\n", return_value2);

     
    FSDK_BStr_Init(&filter_new);
    FSDK_Signature_GetFilter(signature, &filter_new);
    printf("Filter:%s\r\n", filter_new.str);    
    FSDK_BStr_Init(&sub_filter_new);
    FSDK_Signature_GetSubFilter(signature, &sub_filter_new);
    printf("Sub-filter:%s\r\n", sub_filter_new.str);
    FSDK_BStr_Clear(&filter_new);
    FSDK_BStr_Clear(&sub_filter_new);

    if (is_signed) {
      FSDateTime sign_time;
      char datetime_str[100] = { 0 };
      char* time_str = NULL;
      FSDK_Signature_GetSignTime(signature, &sign_time); 
      time_str = DateTimeToString(datetime_str, 100, &sign_time);
      printf("Sign Time:%s\r\n", time_str);
    }

    FSDK_WStr_Init(&key_value_new);
    FSDK_Signature_GetKeyValue(signature, e_FSKeyNameSigner, &key_value_new);
    wprintf(L"Signer:%s\r\n", key_value_new.str);
    FSDK_WStr_Clear(&key_value_new);
    FSDK_Signature_GetKeyValue(signature, e_FSKeyNameLocation, &key_value_new);
    wprintf(L"Location:%s\r\n", key_value_new.str);
    FSDK_WStr_Clear(&key_value_new);
    FSDK_Signature_GetKeyValue(signature, e_FSKeyNameReason, &key_value_new);
    wprintf(L"Reason:%s\r\n", key_value_new.str);
    FSDK_WStr_Clear(&key_value_new);
    FSDK_Signature_GetKeyValue(signature, e_FSKeyNameContactInfo, &key_value_new);
    wprintf(L"Contact Info:%s\r\n", key_value_new.str);
    FSDK_WStr_Clear(&key_value_new);
    FSDK_Signature_GetKeyValue(signature, e_FSKeyNameDN, &key_value_new);
    wprintf(L"DN:%s\r\n", key_value_new.str);
    FSDK_WStr_Clear(&key_value_new);
    FSDK_Signature_GetKeyValue(signature, e_FSKeyNameText, &key_value_new);
    wprintf(L"Text:%s\r\n", key_value_new.str);
    FSDK_WStr_Clear(&key_value_new);
  }
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  wchar_t signed_pdf_path[MAX_FILE_PATH] = { 0 };
  FSErrorCode error_code = 0;
  FS_PDFDOC_HANDLE pdf_doc = NULL;
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  FS_PDFDOC_HANDLE pdf_doc1;
  int i;

  swprintf_s(output_directory, MAX_FILE_PATH, L"%spaging_seal_signature/", output_path);
  _wmkdir(output_directory);  
  swprintf_s(input_file, MAX_FILE_PATH, L"%sAboutFoxit.pdf", input_path);
  // Initialize library
  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    return 1;
  }

  wprintf(L"Input file path: %s\r\n", input_file);
  for (i = 0; i < 2; i++) {
    error_code = FSDK_PDFDoc_Create0(input_file, &pdf_doc);
    if (error_code != e_FSErrSuccess) return 1;

    error_code = FSDK_PDFDoc_StartLoad(pdf_doc, NULL, FALSE, NULL, &progressive);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(pdf_doc);
      wprintf(L"The Doc [%s] Error: %d\n", input_path, error_code);
      return 1;
    }

    // Check information of existed paging seal signature(s) in PDF file if there's any paging seal signature in the PDF file.
    CheckPagingSealSignatureInfo(pdf_doc);
    // Add new paging seal signature, sign it and verify it with filter "Adobe.PPKLite" and different subfilter.  
    AdobePPKLitePagingSealSignature(pdf_doc, i > 0 ? TRUE : FALSE);
    swprintf_s(signed_pdf_path, MAX_FILE_PATH, L"%ssigned_middle_paging_seal_signature.pdf", output_directory);
    if (i > 0)
      swprintf_s(signed_pdf_path, MAX_FILE_PATH, L"%ssigned_left_paging_seal_signature_default_handler.pdf", output_directory);

    error_code = FSDK_PDFDoc_Create0(signed_pdf_path, &pdf_doc1);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_StartLoad(pdf_doc1, NULL, FALSE, NULL, &progressive);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(pdf_doc1);
      wprintf(L"The Doc [%s] Error: %d\n", signed_pdf_path, error_code);
      return 1;
    }
    CheckPagingSealSignatureInfo(pdf_doc1);
  }

  return err_ret;
}

