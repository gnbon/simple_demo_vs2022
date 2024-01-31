// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to do encryption and decryption for a PDF file
// by using password security method, custom security method, Foxit DRM security method, certificate security method
// or Microsoft RMS security method.
//
// NOTE: For Linux and Mac, before using this demo, user should ensure that openssl environment has been prepared:
//       user should ensure all the openssl header files included by "#include" can be found and
//       put the "libssl.a" and "libcrypto.a" in the directory "../../../lib".

#include<direct.h>
#include <windows.h>
#include <math.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_security_c.h"

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

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

wchar_t output_directory[MAX_FILE_PATH];

#include <WinCrypt.h>
#define PSL_ALL_PERMISSION 4294967292
#define PSL_NO_PERMISSION 4294963392
#define PSL_ENCODING_TYPE PKCS_7_ASN_ENCODING | X509_ASN_ENCODING
#define PSL_SEED_LENGTH 20
#define PSL_PDFKEY_LENGTH 16

HMODULE m_hCrypt32 = NULL;
HMODULE m_hCryptui = NULL;
HMODULE m_hAdvapi32 = NULL;

BOOL m_bCrypt32Loaded = FALSE;
BOOL m_bCryptuiLoaded = FALSE;
BOOL m_bAdvapi32Loaded = FALSE;

#if defined(_MSC_VER) && _MSC_VER < 1300
#include "wincrypt.h"
#endif
#define PSL_PASSWORD_MAXLEN 32

HMODULE GetCrypt32() {
  return m_hCrypt32;
}

HMODULE GetAdvapi32() {
  return m_hAdvapi32;
}

#define PSL_MAKEFUNC_CRYPTLIB(dllfunc, funcnameas, funcname, clare, params, rettype, defval) \
rettype funcnameas##clare \
{ \
    HMODULE hModule = ##dllfunc(); \
    if (hModule) { \
      FARPROC proc = GetProcAddress(hModule, #funcname); \
      if (proc) { \
        return proc##params; \
      } \
    } \
    return defval; \
  \
}

//Crypt32
PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CryptMsgUpdate, CryptMsgUpdate, (HCRYPTMSG hCryptMsg, const BYTE *pbData, DWORD cbData, BOOL fFinal),
(hCryptMsg, pbData, cbData, fFinal), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CryptMsgGetParam, CryptMsgGetParam,
(void *hCryptMsg, DWORD dwParamType, DWORD dwIndex, void *pvData, DWORD *pcbData),
(hCryptMsg, dwParamType, dwIndex, pvData, pcbData), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CryptMsgClose, CryptMsgClose, (HCRYPTMSG hCryptMsg), (hCryptMsg), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CryptMsgCalculateEncodedLength, CryptMsgCalculateEncodedLength,
(DWORD dwMsgEncodingType, DWORD dwFlags, DWORD dwMsgType, void const *pvMsgEncodeInfo,
  OPTIONAL LPSTR pszInnerContentObjID, DWORD cbData),
  (dwMsgEncodingType, dwFlags, dwMsgType, pvMsgEncodeInfo, pszInnerContentObjID, cbData), DWORD, 0)

PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CryptMsgOpenToEncode, CryptMsgOpenToEncode,
  (DWORD dwMsgEncodingType, DWORD dwFlags, DWORD dwMsgType, void const *pvMsgEncodeInfo,
    OPTIONAL LPSTR pszInnerContentObjID, OPTIONAL PCMSG_STREAM_INFO pStreamInfo),
    (dwMsgEncodingType, dwFlags, dwMsgType, pvMsgEncodeInfo, pszInnerContentObjID, pStreamInfo),
    HCRYPTMSG, NULL)

PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CertGetIntendedKeyUsage, CertGetIntendedKeyUsage,
  (DWORD dwCertEncodingType, PCERT_INFO pCertInfo, BYTE *pbKeyUsage, DWORD cbKeyUsage),
    (dwCertEncodingType, pCertInfo, pbKeyUsage, cbKeyUsage), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CertDeleteCertificateFromStore, CertDeleteCertificateFromStore, (IN PCCERT_CONTEXT pCertContext), (pCertContext),
    BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CertFreeCertificateContext, CertFreeCertificateContext, (IN PCCERT_CONTEXT pCertContext), (pCertContext), BOOL,
    FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetCrypt32, _CertCreateCertificateContext, CertCreateCertificateContext,
  (IN DWORD dwCertEncodingType, IN const BYTE *pbCertEncoded, IN DWORD cbCertEncoded),
    (dwCertEncodingType, pbCertEncoded, cbCertEncoded), PCCERT_CONTEXT, NULL)

// Advapi32
PSL_MAKEFUNC_CRYPTLIB(GetAdvapi32, _CryptAcquireContextA, CryptAcquireContextA,
  (HCRYPTPROV * phProv, LPCSTR szContainer, LPCSTR szProvider, DWORD dwProvType, DWORD dwFlags),
    (phProv, szContainer, szProvider, dwProvType, dwFlags), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetAdvapi32, _CryptCreateHash, CryptCreateHash,
  (HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH *phHash),
    (hProv, Algid, hKey, dwFlags, phHash), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetAdvapi32, _CryptHashData, CryptHashData,
  (HCRYPTHASH hHash, CONST BYTE *pbData, DWORD dwDataLen, DWORD dwFlags),
    (hHash, pbData, dwDataLen, dwFlags), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetAdvapi32, _CryptGetHashParam, CryptGetHashParam,
  (HCRYPTHASH hHash, DWORD dwParam, BYTE *pbData, DWORD *pdwDataLen, DWORD dwFlags),
    (hHash, dwParam, pbData, pdwDataLen, dwFlags), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetAdvapi32, _CryptDestroyHash, CryptDestroyHash, (HCRYPTHASH hHash), (hHash), BOOL, FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetAdvapi32, _CryptReleaseContext, CryptReleaseContext, (HCRYPTPROV hProv, DWORD dwFlags), (hProv, dwFlags), BOOL,
    FALSE)

PSL_MAKEFUNC_CRYPTLIB(GetAdvapi32, _CryptGenRandom, CryptGenRandom, (HCRYPTPROV hProv, DWORD dwLen, BYTE *pbBuffer),
  (hProv, dwLen, pbBuffer), BOOL, FALSE)

#define PSL_PERMISSION_LENGTH 4
#define PSL_SEEDBUF_LENGTH PSL_SEED_LENGTH + PSL_PERMISSION_LENGTH

FS_BOOL MakeEnvelope(PCERT_INFO* pCertArray, int nCertCount, FS_UINT32 nPermission, FS_UINT8* pSeed,
    FS_UINT8** pbEncodedBlob, FS_UINT32* cbEncodedBlob) {
  HCRYPTPROV hcp = 0;
  CRYPT_ALGORITHM_IDENTIFIER ContentEncryptAlgorithm;
  BOOL bRet = FALSE;
  DWORD ContentEncryptAlgSize = 0;
  CMSG_ENVELOPED_ENCODE_INFO EnvelopedEncodeInfo;
  HCRYPTMSG hMsg;
  FS_UINT8 szSeedBuf[PSL_SEEDBUF_LENGTH] = { 0 };
  int lasterr = 0;

  bRet = CryptAcquireContextA(&hcp, NULL, NULL, 1, 0);
  if (!bRet)
    return FALSE;

  // Initialize the symmetric-encryption algorithm identifier structure.

  ContentEncryptAlgSize = sizeof(ContentEncryptAlgorithm);
  memset(&ContentEncryptAlgorithm, 0, ContentEncryptAlgSize);

  // Initialize the necessary members. This particular OID does not need any parameters.
  // Some OIDs, however, will require that the other members be initialized.
  ContentEncryptAlgorithm.pszObjId = szOID_RSA_RC4;

  // Initialize the CMSG_ENVELOPED_ENCODE_INFO structure.
  memset(&EnvelopedEncodeInfo, 0, sizeof(CMSG_ENVELOPED_ENCODE_INFO));
  EnvelopedEncodeInfo.cbSize = sizeof(CMSG_ENVELOPED_ENCODE_INFO);
  EnvelopedEncodeInfo.hCryptProv = (HCRYPTPROV)hcp;
  EnvelopedEncodeInfo.ContentEncryptionAlgorithm = ContentEncryptAlgorithm;
  EnvelopedEncodeInfo.pvEncryptionAuxInfo = NULL;
  EnvelopedEncodeInfo.cRecipients = nCertCount;
  EnvelopedEncodeInfo.rgpRecipients = pCertArray;

  // Get the size of the encoded message BLOB.
  *cbEncodedBlob = _CryptMsgCalculateEncodedLength(PSL_ENCODING_TYPE,     // message encoding type
    0,                     // flags
    CMSG_ENVELOPED,        // message type
    &EnvelopedEncodeInfo,  // pointer to structure
    NULL,                  // inner content OID
    24);                   // size of content
  if (cbEncodedBlob == 0) {
    CryptReleaseContext(hcp, 0);
    return FALSE;
  }

  // Allocate memory for the encoded BLOB.
  *pbEncodedBlob = (FS_UINT8*)malloc(*cbEncodedBlob);
  if (!(*pbEncodedBlob)) {
    CryptReleaseContext(hcp, 0);
    return FALSE;
  }

  // Open a message to encode.
  hMsg = _CryptMsgOpenToEncode(PSL_ENCODING_TYPE,     // encoding type
    0,                     // flags
    CMSG_ENVELOPED,        // message type
    &EnvelopedEncodeInfo,  // pointer to structure
    NULL,                  // inner content OID
    NULL);                 // stream information (not used)

  if (hMsg == NULL) {
    CryptReleaseContext(hcp, 0);
    free(*pbEncodedBlob);
    return FALSE;
  }

  // Data to be Encrypted
  memcpy(szSeedBuf, pSeed, PSL_SEED_LENGTH);

  *(szSeedBuf + PSL_SEED_LENGTH) = HIBYTE(HIWORD(nPermission));
  *(szSeedBuf + PSL_SEED_LENGTH + 1) = LOBYTE(HIWORD(nPermission));
  *(szSeedBuf + PSL_SEED_LENGTH + 2) = HIBYTE(LOWORD(nPermission));

  if (nPermission == PSL_ALL_PERMISSION)
    *(szSeedBuf + PSL_SEED_LENGTH + 3) = LOBYTE(LOWORD(nPermission)) | 0x03;
  else
    *(szSeedBuf + PSL_SEED_LENGTH + 3) = LOBYTE(LOWORD(nPermission)) | 0x01;

  lasterr = GetLastError();
  // Update the message with the data.
  bRet = _CryptMsgUpdate(hMsg, szSeedBuf, PSL_SEEDBUF_LENGTH, TRUE);
  if (!bRet) {
    _CryptMsgClose(hMsg);
    _CryptReleaseContext(hcp, 0);
    free(*pbEncodedBlob);

    return FALSE;
  }

  // Get the resulting message.
  bRet = _CryptMsgGetParam(hMsg, CMSG_CONTENT_PARAM, 0, *pbEncodedBlob, (DWORD*)&cbEncodedBlob);

  if (!bRet) {
    _CryptMsgClose(hMsg);
    _CryptReleaseContext(hcp, 0);
    free(pbEncodedBlob);

    return FALSE;
  }

  // Clean up.
  _CryptMsgClose(hMsg);
  _CryptReleaseContext(hcp, 0);
  return TRUE;
}

void CreateSeed(FS_UINT8* pSeed) {
  HCRYPTPROV hcp = 0;
  DWORD dwErrCode = 0;

  CryptAcquireContextA(&hcp, NULL, NULL, 1, 0);

  dwErrCode = GetLastError();
  if (dwErrCode == NTE_BAD_KEYSET) {
    _CryptAcquireContextA(&hcp, NULL, NULL, 1, CRYPT_DELETEKEYSET);
    _CryptAcquireContextA(&hcp, NULL, NULL, 1, CRYPT_NEWKEYSET);
  }

  if (hcp) {
    _CryptGenRandom(hcp, PSL_SEED_LENGTH, pSeed);
    _CryptReleaseContext(hcp, 0);
  }
}

void GetPDFKeyFromEnvelopes(FS_BSTR* pEnvelopes, int envelopesCount, const FS_UINT8* pSeed,
  FS_BOOL bEncryptMeta, FS_UINT8* pPDFKey) {
  // Calculate encryption key
  HCRYPTPROV hcp = 0;
  HCRYPTHASH hash = 0;
  int i = 0 ,sz;
  DWORD size = 20;

  // Acquire Context http://support.microsoft.com/kb/238187/zh-cn
  if (!CryptAcquireContextA(&hcp, "Container", NULL, PROV_RSA_FULL, 0)) {
    if (GetLastError() == NTE_BAD_KEYSET) {
      if (!CryptAcquireContextA(&hcp, ("Container"), NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET)) {
      }
    }
  }

  if (!hcp)
    return;

  _CryptCreateHash(hcp, (4 << 13) | 4, 0, 0, &hash);

  _CryptHashData(hash, pSeed, 20, 0);

  for (i = 0, sz = envelopesCount; i < sz; i++) {
    _CryptHashData(hash, (BYTE*)pEnvelopes[i].str, pEnvelopes[i].len, 0);
  }

  if (!bEncryptMeta) {
    FS_UINT32 tag = 0xffffffff;
    _CryptHashData(hash, (const BYTE*)&tag, 4, 0);
  }

  _CryptGetHashParam(hash, 2, pPDFKey, &size, 0);

  _CryptDestroyHash(hash);
  _CryptReleaseContext(hcp, 0);
}

FS_BOOL GetCertificateInfo(const wchar_t* certFilePath, FS_BSTR* envelopes, int* envelopeslength, FS_BSTR* initialKey, FS_BOOL bEncryptMetaData,
  int len) {
  FILE* fpCert = NULL;
  SYSTEMTIME st;
  long cbData = 0;
  FS_UINT8* pbData = NULL;
  BYTE usage;
  FILETIME crtTime;
  FS_UINT8 Seed[PSL_SEED_LENGTH];
  PCCERT_CONTEXT pTempCertContext;
  FS_UINT8* pbEncodedBlob = NULL;
  FS_UINT32 cbEncodedBlob = 0;
  PCERT_INFO pCertInfo;
  FS_BSTR tempEnvolops[1];
  FS_UINT8 PDFKey[128] = {0};

  _wfopen_s(&fpCert, certFilePath, L"rb");
  if (!fpCert) {
    return FALSE;
  }
  fseek(fpCert, 0, SEEK_END);
  cbData = ftell(fpCert);
  pbData = (FS_UINT8*)malloc(sizeof(FS_UINT8) * cbData);
  fseek(fpCert, 0, SEEK_SET);
  fread(pbData, sizeof(char), cbData, fpCert);
  fclose(fpCert);

  pTempCertContext = _CertCreateCertificateContext(PSL_ENCODING_TYPE, pbData, cbData);
  if (!pTempCertContext)
    return FALSE;

  _CertGetIntendedKeyUsage(pTempCertContext->dwCertEncodingType, pTempCertContext->pCertInfo, &usage, 1);
  if ((usage & CERT_DATA_ENCIPHERMENT_KEY_USAGE) != CERT_DATA_ENCIPHERMENT_KEY_USAGE && usage != 0) {
    _CertFreeCertificateContext(pTempCertContext);
    return FALSE;
  }

  GetLocalTime(&st);
  SystemTimeToFileTime(&st, &crtTime);
  if (CompareFileTime(&crtTime, &(pTempCertContext->pCertInfo->NotBefore)) < 0 ||
    CompareFileTime(&crtTime, &(pTempCertContext->pCertInfo->NotAfter)) > 0) {
    _CertFreeCertificateContext(pTempCertContext);
    return FALSE;
  }

  CreateSeed(Seed);
  pCertInfo = pTempCertContext->pCertInfo;
  if (!MakeEnvelope(&pCertInfo, 1, 0xfffff8c4, Seed, &pbEncodedBlob, &cbEncodedBlob))
    return FALSE;
  _CertFreeCertificateContext(pTempCertContext);

  tempEnvolops[0].str = (char*)(pbEncodedBlob);
  tempEnvolops[0].len = cbEncodedBlob;

  GetPDFKeyFromEnvelopes(tempEnvolops, 1, Seed, bEncryptMetaData, PDFKey);
  *envelopeslength = 1;
  if (envelopes) {
    FSDK_BStr_SetLength(&envelopes[0], tempEnvolops[0].len);
    FSDK_BStr_Set(&envelopes[0], tempEnvolops[0].str, tempEnvolops[0].len);
  }

  FSDK_BStr_SetLength(initialKey, len);
  FSDK_BStr_Set(initialKey, (char*)PDFKey, len);

  free(pbEncodedBlob);
  free(pbData);

  return TRUE;
}

FS_BOOL CertificateSecurity(const wchar_t* input_file) {
  FS_PDFDOC_HANDLE doc = NULL;
  FS_BSTR initial_key;
  wchar_t cert_file_path[MAX_FILE_PATH];
  int envelopeslength = 1;
  FS_BSTR* envelopes = NULL;
  FS_CERTIFICATESECURITYHANDLER_HANDLE handler = NULL;
  FSCertificateEncryptData encrypt_data;
  FS_BOOL result;
  wchar_t output_file[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE encrypted_doc = NULL;
  FSErrorCode code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return 1;
  }

  if (!m_bCrypt32Loaded) {
    m_hCrypt32 = LoadLibraryW(L"crypt32.dll");
    m_bCrypt32Loaded = TRUE;
  }
  if (!m_bCryptuiLoaded) {
    m_hCryptui = LoadLibraryW(L"cryptui.dll");
    m_bCryptuiLoaded = TRUE;
  }

  if (!m_bAdvapi32Loaded) {
    m_hAdvapi32 = LoadLibraryW(L"advapi32.dll");
    m_bAdvapi32Loaded = TRUE;
  }

  // Do encryption.
  FSDK_BStr_Init(&initial_key);
  swprintf_s(cert_file_path, MAX_FILE_PATH, L"%lsfoxit.cer", input_path);

  envelopes = (FS_BSTR*)malloc(sizeof(FS_BSTR) * envelopeslength);
  if (!GetCertificateInfo(cert_file_path, envelopes, &envelopeslength, &initial_key, TRUE, 16)) {
    wprintf(L"[Failed] Cannot get certificate information from %ls\r\n", cert_file_path);
    return FALSE;
  }

  FSDK_CertificateSecurityHandler_Create(&handler);

  encrypt_data.cipher = e_FSCipherAES;
  encrypt_data.is_encrypt_metadata = TRUE;
  encrypt_data.envelopes = envelopes;
  encrypt_data.envelopes_length = envelopeslength;

  FSDK_CertificateSecurityHandler_Initialize(handler, encrypt_data, initial_key.str, &result);

  FSDK_PDFDoc_SetSecurityHandler(doc, handler, &result);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lscertificate_encrypt.pdf", output_directory);

  FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &result);
  free(envelopes);
  FSDK_PDFDoc_Release(doc);

  FSDK_PDFDoc_Create0(output_file, &encrypted_doc);
  code = FSDK_PDFDoc_Load(encrypted_doc, NULL);
  if (code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", output_file, code);
    return FALSE;
  }
  swprintf_s(output_file, MAX_FILE_PATH, L"%lscertificate_decrypt.pdf", output_directory);

  FSDK_PDFDoc_RemoveSecurity(encrypted_doc, &result);
  FSDK_PDFDoc_SaveAs(encrypted_doc, output_file, e_FSSaveFlagNoOriginal, &result);

  FSDK_SecurityHandler_Release(handler);
  FSDK_PDFDoc_Release(encrypted_doc);

  if (m_hCrypt32) {
    FreeLibrary(m_hCrypt32);
    m_hCrypt32 = NULL;
  }

  if (m_hCryptui) {
    FreeLibrary(m_hCryptui);
    m_hCryptui = NULL;
  }

  if (m_hAdvapi32) {
    FreeLibrary(m_hAdvapi32);
    m_hAdvapi32 = NULL;
  }
  return result;
}

static void gRelease(void* user_data) {
  free(user_data);
}

static FSEncryptType gDRMGetSecurityType(void* user_data) { return e_FSEncryptFoxitDRM; }

static FS_BOOL gIsOwner(void* user_data, const FS_PDFDOC_HANDLE doc, const char* subFilter) {
  return TRUE;
}

static FS_UINT32 gGetUserPermissions(void* user_data, const FS_PDFDOC_HANDLE doc, const char* subFilter) {
  return 0xFFFFFFFC;
}

static FSCipherType gGetCipherType(void* user_data, const FS_PDFDOC_HANDLE doc, const char* subFilter) {
  return e_FSCipherAES;
}

static FS_UINT32 gGetKeyLength(void* user_data, const FS_PDFDOC_HANDLE doc, const char* subFilter) {
  return 16;
}

static FS_BSTR gGetFileID(void* user_data, const FS_PDFDOC_HANDLE doc, const char* subFilter) {
  FS_BSTR str;
  str.str = "Simple-DRM-file-ID";
  str.len = strlen("Simple-DRM-file-ID");
  return str;
}
  
static FS_BSTR gGetInitialKey(void* user_data, const FS_PDFDOC_HANDLE doc, const char* subFilter) {
  FS_BSTR str;
  str.str = "Simple-DRM-initialize-key";
  str.len = strlen("Simple-DRM-initialize-key");
  return str;
}

FS_BOOL FoxitDRMDecrypt(const wchar_t* input_file) {
  FS_PDFDOC_HANDLE doc = NULL;
  FS_DRMSECURITYHANDLER_HANDLE handler = NULL;
  const char* file_id = "Simple-DRM-file-ID";
  const char* initialize_key = "Simple-DRM-initialize-key";
  FSDRMEncryptData encrypt_data;
  FS_BOOL result;
  wchar_t output_file[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE encrypted_doc = NULL;
  FSDRMSecurityCallback* callback = NULL;
  FSSecurityCallback* security_callback = NULL;

  FSErrorCode code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (code != e_FSErrSuccess) return FALSE;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return FALSE;
  }

  // Do encryption.

  FSDK_DRMSecurityHandler_Create(&handler);

  encrypt_data.is_encrypt_metadata = TRUE;
  encrypt_data.cipher = e_FSCipherAES;
  encrypt_data.is_owner = TRUE;
  encrypt_data.key_length = 16;
  encrypt_data.sub_filter.str = "Simple-DRM-filter";
  encrypt_data.sub_filter.len = strlen("Simple-DRM-filter");
  encrypt_data.user_permissions = 0xfffffffc;

  FSDK_DRMSecurityHandler_Initialize(handler, encrypt_data, file_id, initialize_key, &result);
  FSDK_PDFDoc_SetSecurityHandler(doc, handler, &result);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsfoxit_drm_encrypt.pdf", output_directory);

  FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &result);
  FSDK_PDFDoc_Release(doc);

  // Do decryption.
  callback = (FSDRMSecurityCallback*)malloc(sizeof(FSDRMSecurityCallback));
  security_callback = (FSSecurityCallback*)malloc(sizeof(FSSecurityCallback));

  callback->user_data = callback;
  callback->GetCipherType = gGetCipherType;
  callback->securitycallback = security_callback;
  callback->securitycallback->user_data = security_callback;
  callback->securitycallback->GetSecurityType = gDRMGetSecurityType;
  callback->securitycallback->Release = gRelease;
  callback->GetFileID = gGetFileID;
  callback->GetInitialKey = gGetInitialKey;
  callback->GetKeyLength = gGetKeyLength;
  callback->GetUserPermissions = gGetUserPermissions;
  callback->IsOwner = gIsOwner;
  
  FSDK_Library_RegisterSecurityCallback("FoxitDRM", callback, &result);

  FSDK_PDFDoc_Create0(output_file, &encrypted_doc);
  code = FSDK_PDFDoc_Load(encrypted_doc, NULL);
  if (code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", output_file, code);
    return FALSE;
  }
  swprintf_s(output_file, MAX_FILE_PATH, L"%lsfoxit_drm_decrypt.pdf", output_directory);

  FSDK_PDFDoc_RemoveSecurity(encrypted_doc, &result);
  FSDK_PDFDoc_SaveAs(encrypted_doc, output_file, e_FSSaveFlagNoOriginal, &result);

  FSDK_SecurityHandler_Release(handler);
  FSDK_PDFDoc_Release(encrypted_doc);

  FSDK_Library_UnregisterSecurityCallback("FoxitDRM", &result);

  return TRUE;
}

static FSEncryptType gCustomGetSecurityType(void* user_data) {
  return e_FSEncryptCustom;
}

static FSEncryptType gRMSGetSecurityType(void* user_data) {
  return e_FSEncryptRMS;
}

static void* gCreateContext(void* user_data, const char* filter, const char* sub_filter, const char* encrypt_info) {
  return NULL;
}

static FS_BOOL gReleaseContext(void* user_data, void* context) {
  return TRUE;
}

static FSCipherType gGetCipher(void* user_data, const void* context) {
  return e_FSCipherAES;
}

static FS_BSTR gGetEncryptKey(void* user_data, const void* context) {
  FS_BSTR str;
  str.str = "key";
  str.len = 3;
  return str;
}

static FS_UINT32 gGetDecryptedSize(void* user_data, const void* context, FS_UINT32 data_size) {
  return data_size;
}

static void* gStartDecryptor(void* user_data, const void* context, int obj_number, int gen_number) {
  return NULL;
}

static FS_BSTR gDecryptData(void* user_data, const void* decryptor, const void* enc_data_buf, FS_UINT32 enc_data_length) {
  FS_UINT32 i = 0; 
  FS_BSTR str;
  char* decrypted_data = (char*)malloc(enc_data_length);
  for (i = 0; i < enc_data_length; i++)
    ((char*)decrypted_data)[i] = ((char*)enc_data_buf)[i] + 1;

  FSDK_BStr_Init(&str);
  FSDK_BStr_SetLength(&str, enc_data_length);
  FSDK_BStr_Set(&str, decrypted_data, enc_data_length);
  return str;
}

static FS_BSTR gFinishDecryptor(void* user_data, void* decryptor) {
  FS_BSTR str;
  FSDK_BStr_Init(&str);
  return str;
}

static FS_UINT32 gGetEncryptedSize(void* user_data, const void* context, int obj_number, int gen_number, const void* src_buffer,
    FS_UINT32 src_length) {
  return src_length;
}

static FS_BOOL gEncryptData(void* user_data, const void* context, int obj_number, int gen_number, const void* src_buffer,
  FS_UINT32 src_length, void* dst_bufffer, FS_UINT32* dst_length) {
  FS_UINT32 i = 0;
  for (i = 0; i < src_length; i++)
    ((char*)dst_bufffer)[i] = ((char*)src_buffer)[i] - 1;
  *dst_length = src_length;
  return TRUE;
}

static FS_BOOL gUseStandardCryptoMethod(void* user_data, const void* context) { return FALSE; }

FS_BOOL CustomSecurity(const wchar_t* input_file) {
  FS_PDFDOC_HANDLE doc = NULL;
  FS_CUSTOMSECURITYHANDLER_HANDLE handler = NULL;
  FSCustomSecurityCallback* custom_security_callback = NULL;
  FSSecurityCallback* security_callback = NULL;
  const char* encrypt_info = "Foxit simple demo custom security";
  FSCustomEncryptData encrypt_data;

  FS_BOOL result;
  wchar_t output_file[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE encrypted_doc = NULL;

  FSErrorCode code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return 1;
  }

  // Do encryption.
  custom_security_callback = (FSCustomSecurityCallback*)malloc(sizeof(FSCustomSecurityCallback));
  security_callback = (FSSecurityCallback*)malloc(sizeof(FSSecurityCallback));

  custom_security_callback->user_data = custom_security_callback;
  custom_security_callback->CreateContext = gCreateContext;
  custom_security_callback->DecryptData = gDecryptData;
  custom_security_callback->FinishDecryptor = gFinishDecryptor;
  custom_security_callback->GetCipher = gGetCipher;
  custom_security_callback->GetDecryptedSize = gGetDecryptedSize;
  custom_security_callback->GetEncryptedSize = gGetEncryptedSize;
  custom_security_callback->GetEncryptKey = gGetEncryptKey;
  custom_security_callback->securitycallback = security_callback;
  custom_security_callback->securitycallback->user_data = security_callback;
  custom_security_callback->securitycallback->GetSecurityType = gCustomGetSecurityType;
  custom_security_callback->securitycallback->Release = gRelease;
  custom_security_callback->UseStandardCryptoMethod = gUseStandardCryptoMethod;
  custom_security_callback->StartDecryptor = gStartDecryptor;
  custom_security_callback->EncryptData = gEncryptData;

  FSDK_CustomSecurityHandler_Create(&handler);

  encrypt_data.is_encrypt_metadata = TRUE;
  encrypt_data.filter.str = "Custom";
  encrypt_data.filter.len = strlen("Custom");
  encrypt_data.sub_filter.str = "foxit-simple-demo";
  encrypt_data.sub_filter.len = strlen("foxit-simple-demo");

  FSDK_CustomSecurityHandler_Initialize(handler, encrypt_data, custom_security_callback, encrypt_info, &result);
  FSDK_PDFDoc_SetSecurityHandler(doc, handler, &result);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lscustom_encrypt.pdf", output_directory);

  FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &result);

  // Do decryption.
  FSDK_Library_RegisterSecurityCallback("Custom", custom_security_callback, &result);

  FSDK_PDFDoc_Create0(output_file, &encrypted_doc);
  code = FSDK_PDFDoc_Load(encrypted_doc, NULL);
  if (code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", output_file, code);
    return FALSE;
  }
  swprintf_s(output_file, MAX_FILE_PATH, L"%lscustom_decrypt.pdf", output_directory);
  FSDK_PDFDoc_RemoveSecurity(encrypted_doc, &result);
  FSDK_PDFDoc_SaveAs(encrypted_doc, output_file, e_FSSaveFlagNoOriginal, &result);

  FSDK_SecurityHandler_Release(handler);
  FSDK_PDFDoc_Release(encrypted_doc);
  FSDK_PDFDoc_Release(doc);
  FSDK_Library_UnregisterSecurityCallback("Custom", &result);
  return result;
}

static FS_BSTR gRMSGetEncryptKey(void* user_data, const void* context) {
  FS_BSTR str;
  str.str = "Simple-RMS-encrpty-key";
  str.len = strlen("Simple-RMS-encrpty-key");
  return str;
}

FS_BOOL RMSSecurity(const wchar_t* input_file) {
  FS_PDFDOC_HANDLE doc = NULL;
  FS_RMSSECURITYHANDLER_HANDLE handler = NULL;
  FS_BSTR server_eul_list[4];
  FSRMSEncryptData encrypt_data;
  FS_BOOL result;
  FS_PDFDOC_HANDLE encrypted_doc = NULL;
  wchar_t output_file[MAX_FILE_PATH];
  FSErrorCode code = e_FSErrSuccess;
  FSRMSSecurityCallback* callback = NULL;
  FSCustomSecurityCallback* custom_security_callback = NULL;
  FSSecurityCallback* security_callback = NULL;

  code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (code != e_FSErrSuccess) return FALSE;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return FALSE;
  }

  // Do encryption.
  callback = (FSRMSSecurityCallback*)malloc(sizeof(FSRMSSecurityCallback));
  custom_security_callback = (FSCustomSecurityCallback*)malloc(sizeof(FSCustomSecurityCallback));
  security_callback = (FSSecurityCallback*)malloc(sizeof(FSSecurityCallback));

  callback->user_data = callback;
  callback->customsecuritycallback = custom_security_callback;
  callback->customsecuritycallback->user_data = custom_security_callback;
  callback->customsecuritycallback->CreateContext = gCreateContext;
  callback->customsecuritycallback->DecryptData = gDecryptData;
  callback->customsecuritycallback->FinishDecryptor = gFinishDecryptor;
  callback->customsecuritycallback->GetCipher = gGetCipher;
  callback->customsecuritycallback->GetDecryptedSize = gGetDecryptedSize;
  callback->customsecuritycallback->GetEncryptedSize = gGetEncryptedSize;
  callback->customsecuritycallback->GetEncryptKey = gRMSGetEncryptKey;
  callback->customsecuritycallback->securitycallback = security_callback;
  callback->customsecuritycallback->securitycallback->user_data = security_callback;
  callback->customsecuritycallback->securitycallback->GetSecurityType = gRMSGetSecurityType;
  callback->customsecuritycallback->securitycallback->Release = gRelease;
  callback->customsecuritycallback->UseStandardCryptoMethod = gUseStandardCryptoMethod;
  callback->customsecuritycallback->EncryptData = gEncryptData;
  callback->customsecuritycallback->StartDecryptor = gStartDecryptor;

  FSDK_RMSSecurityHandler_Create(&handler);

  server_eul_list[0].str = "WM-1";
  server_eul_list[0].len = strlen("WM-1");
  server_eul_list[1].str = "This document has been encrypted by RMS encryption.";
  server_eul_list[1].len = strlen("This document has been encrypted by RMS encryption.");
  server_eul_list[2].str = "WM-2";
  server_eul_list[2].len = strlen("WM-2");
  server_eul_list[3].str = "Just for simple demo rms security.";
  server_eul_list[3].len = strlen("Just for simple demo rms security.");

  encrypt_data.is_encrypt_metadata = TRUE;
  encrypt_data.publish_license.str = "PubishLicense_0123";
  encrypt_data.publish_license.len = strlen("PubishLicense_0123");
  encrypt_data.irm_version = 1;
  encrypt_data.server_eul_list = server_eul_list;
  encrypt_data.server_eul_list_length = 4;

  FSDK_RMSSecurityHandler_Initialize(handler, encrypt_data, callback, &result);
  FSDK_PDFDoc_SetSecurityHandler(doc, handler, &result);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsrms_encrypt.pdf", output_directory);
  FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &result);

  // Do decryption.
  FSDK_Library_RegisterSecurityCallback("MicrosoftIRMServices", callback, &result);

  FSDK_PDFDoc_Create0(output_file, &encrypted_doc);
  code = FSDK_PDFDoc_Load(encrypted_doc, NULL);
  if (code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", output_file, code);
    return FALSE;
  }

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsrms_decrypt.pdf", output_directory);

  FSDK_PDFDoc_RemoveSecurity(encrypted_doc, &result);
  FSDK_PDFDoc_SaveAs(encrypted_doc, output_file, e_FSSaveFlagNoOriginal, &result);

  FSDK_SecurityHandler_Release(handler);
  FSDK_PDFDoc_Release(encrypted_doc);

  FSDK_Library_UnregisterSecurityCallback("Simple-RMS-encrpty-key", &result);
  FSDK_PDFDoc_Release(doc);
  return result;
}

FS_BOOL StdSecurity(const wchar_t* input_file) {
  FS_PDFDOC_HANDLE doc = NULL;
  FS_STDSECURITYHANDLER_HANDLE handler = NULL;
  FSErrorCode code = e_FSErrSuccess;
  FSStdEncryptData encrypt_data;
  FS_BOOL result;
  wchar_t output_file[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE encrypted_doc = NULL;
  FS_BSTR password;

  code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return 1;
  }

  // Do encryption.
  FSDK_StdSecurityHandler_Create(&handler);

  encrypt_data.is_encrypt_metadata = TRUE;
  encrypt_data.cipher = e_FSCipherAES;
  encrypt_data.key_length = 16;
  encrypt_data.user_permissions = 0xfffffffc;

  FSDK_StdSecurityHandler_Initialize(handler, encrypt_data, "user", "owner", &result);
  FSDK_PDFDoc_SetSecurityHandler(doc, handler, &result);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsstd_encrypt_userpwd[user]_ownerpwd[owner].pdf", output_directory);
  FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &result);

  FSDK_PDFDoc_Create0(output_file, &encrypted_doc);

  password.str = "owner";
  password.len = strlen("owner");
  code = FSDK_PDFDoc_Load(encrypted_doc, &password);
  if (code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", output_file, code);
    return FALSE;
  }

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsstd_decrypt.pdf", output_directory);

  FSDK_PDFDoc_RemoveSecurity(encrypted_doc, &result);
  FSDK_PDFDoc_SaveAs(encrypted_doc, output_file, e_FSSaveFlagNoOriginal, &result);

  FSDK_SecurityHandler_Release(handler);
  FSDK_PDFDoc_Release(encrypted_doc);

  return TRUE;
}

FS_BOOL SecurityImpl(const wchar_t* pdf_file) {
  FS_BOOL is_ok = TRUE;
  if (!CertificateSecurity(pdf_file)) {
    is_ok = FALSE;
  }
  if (!FoxitDRMDecrypt(pdf_file)) {
    is_ok = FALSE;
  }
  if (!CustomSecurity(pdf_file)) {
    is_ok = FALSE;
  }
  if (!RMSSecurity(pdf_file)) {
    is_ok = FALSE;
  }
  if (!StdSecurity(pdf_file)) {
    is_ok = FALSE;
  }

  return is_ok;
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t input_file[MAX_FILE_PATH] = {0};

  FSErrorCode error_code = e_FSErrSuccess;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }
  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lssecurity/", output_path);
  _wmkdir(output_directory);

  if (!SecurityImpl(input_file)) {
    return 1;
  }

  FSDK_Library_Release();

  printf("Security demo test.\r\n");
  return err_ret;
}
