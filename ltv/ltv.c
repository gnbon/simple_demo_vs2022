// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to do LTV verification (using default callback) in PDF document.

// Include header files.
#include <time.h>
#include<direct.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

// Include Foxit SDK header files.
#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_annot_c.h"
#include "../../../include/fs_image_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_signature_c.h"
#include "../../../include/fs_ltvverifier_c.h"


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

// sn and key information from Foxit PDF SDK's key files are used to initialize Foxit PDF SDK library. 
static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";


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


FSDateTime GetLocalDateTime() {
  time_t t = time(NULL);
  struct tm _Tm;
  struct tm* rime;
  long time_zone = 0;
  int timezone_hour = 0;
  int timezone_minute = 0;
  FSDateTime datetime;
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


wchar_t output_directory[MAX_FILE_PATH] = { 0 };

#define FREE_ETSIRFC316TSA_SERVER_NAME L"FreeTSAServer"
#define FREE_ETSIRFC316TSA_SERVER_URL L"http://ca.signfiles.com/TSAServer.aspx"

#define PKCS7_SIGNATURE_FILTER "Adobe.PPKLite"
#define PKCS7_SIGNATURE_SUBFILTER "adbe.pkcs7.detached"


FILE* OpenFileWrapper(const char* file_name, const char* file_mode) {
  FILE* ret_file = NULL;
  fopen_s(&ret_file, (const char*)file_name, (const char*)file_mode);

  return ret_file;
}

char*  TransformSignatureStateToString(char* str_state, size_t state_size, FS_UINT32 sig_state) {
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
  for (i = 0; i < num; i++) {
    if (sig_state & state[i]) {
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

void PKCS7Signature(wchar_t* input_pdf_path, wchar_t* signed_pdf_path) {
  FS_PDFDOC_HANDLE pdf_doc = NULL;
  FS_PROGRESSIVE_HANDLE progressive;
  FS_PDFPAGE_HANDLE pdf_page;
  float page_width;
  float page_height;
  FSRectF new_sig_rect;
  FS_SIGNATURE_HANDLE new_signature;
  char strtmp[100] = { 0 };
  wchar_t new_value[200] = { 0 };
  wchar_t cert_path[MAX_FILE_PATH] = { 0 };
  const wchar_t* cert_password = L"123456";
  FS_PROGRESSIVE_HANDLE sign_progressive;
  FS_UINT32 ap_flags;
  int rate;
  FSDateTime sign_time;

  FSDK_PDFDoc_Create0(input_pdf_path, &pdf_doc);  
  FSDK_PDFDoc_StartLoad(pdf_doc, NULL, TRUE, NULL, &progressive);  
  FSDK_PDFDoc_GetPage(pdf_doc, 0, &pdf_page);  
  FSDK_PDFPage_GetWidth(pdf_page, &page_width);
  FSDK_PDFPage_GetHeight(pdf_page, &page_height);
  new_sig_rect.left = page_width / 2 - 50.0f;
  new_sig_rect.bottom = page_height / 2 - 50.0f;
  new_sig_rect.right = page_width / 2 + 50.0f;
  new_sig_rect.top = page_height / 2 + 50.0f;  
  FSDK_PDFPage_AddSignature(pdf_page, new_sig_rect, &new_signature);
  FSDK_Signature_SetFilter(new_signature, PKCS7_SIGNATURE_FILTER);
  FSDK_Signature_SetSubFilter(new_signature, PKCS7_SIGNATURE_SUBFILTER);

  //String new_value;
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameSigner, L"Foxit PDF SDK");
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameContactInfo, L"support@foxitsoftware.com");
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameDN, L"CN=CN,MAIL=MAIL@MAIL.COM");
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameLocation, L"Fuzhou, China");
 
  sprintf_s(strtmp, 100, "As a sample for subfilter \"%s\" ", PKCS7_SIGNATURE_SUBFILTER);
  string2wstring(strtmp, strlen(strtmp), new_value, 200);
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameReason, new_value);
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameText, new_value);
  sign_time = GetLocalDateTime();
  FSDK_Signature_SetSignTime(new_signature, sign_time);
  // Set appearance flags to decide which content would be used in appearance.
  ap_flags = e_FSAPFlagLabel | e_FSAPFlagSigner |
    e_FSAPFlagReason | e_FSAPFlagDN |
    e_FSAPFlagLocation | e_FSAPFlagText |
    e_FSAPFlagSigningTime;
  FSDK_Signature_SetAppearanceFlags(new_signature, ap_flags);

  swprintf_s(cert_path, MAX_FILE_PATH, L"%sfoxit_all.pfx", input_path);  
  FSDK_Signature_StartSign(new_signature, cert_path, cert_password, e_FSDigestSHA256, signed_pdf_path, NULL, NULL, &sign_progressive);

  FSDK_Progressive_GetRateOfProgress(sign_progressive, &rate);
  if (rate != 100) {
    FSState state;
    FSDK_Progressive_Continue(sign_progressive, &state);
    if (e_FSFinished != state) {
      printf("[Failed] Fail to sign the new CAdES signature.\r\n");
      return;
    }
  }
}


// trust all input certificate(s) when this callback function is triggered during LTV verification.
// User can improve the implementation of the callback class TrustedCertStoreCallback or choose not to use TrustedCertStoreCallback.
static FS_BOOL gIsCertTrusted(void* user_data, const char*cert) {
  return TRUE;
}

static FS_BOOL gIsCertTrustedRoot(void* user_data, const char*cert) {
  return TRUE;
}


void UseLTVVerifier(FS_PDFDOC_HANDLE pdf_doc, FS_BOOL is_to_add_dss) {
  // Here use default RevocationCallback, so no need to call LTVVerifier::SetRevocationCallback
  FS_LTVVERIFIER_HANDLE ltv_verifier;
  FS_UINT32 length = 0;
  FSTrustedCertStoreCallback my_callback;
  FS_SIGNATUREVERIFYRESULT_HANDLE* sig_verify_result_array = NULL;
  size_t i;

  FSDK_LTVVerifier_Create(pdf_doc, TRUE, TRUE, FALSE, e_FSSignatureCreationTime, &ltv_verifier);
  // Use implemented TrustedCertStoreCallback to trust some cerificates during LTV verification.
  // Here, the implementation of TrustedCertStoreCallback is very simple : 
  // trust all input certificate(s) when this callback function is triggered during LTV verification.
  // User can improve the implementation of the callback class TrustedCertStoreCallback or choose not to use TrustedCertStoreCallback.
  my_callback.IsCertTrusted = gIsCertTrusted;
  my_callback.IsCertTrustedRoot = gIsCertTrustedRoot;
  my_callback.user_data = NULL;
  FSDK_LTVVerifier_SetTrustedCertStoreCallback(ltv_verifier, &my_callback);
  FSDK_LTVVerifier_SetVerifyMode(ltv_verifier, e_FSVerifyModeAcrobat);
  FSDK_LTVVerifier_Verify(ltv_verifier, NULL, &length);
  sig_verify_result_array = (FS_SIGNATUREVERIFYRESULT_HANDLE*)malloc(sizeof(FS_SIGNATUREVERIFYRESULT_HANDLE)* length);
  FSDK_LTVVerifier_Verify(ltv_verifier, sig_verify_result_array, &length);
  for (i = 0; i < length; i++) {
    char sig_str[1000] = { 0 };
    size_t sig_size = 1000;
    char ltv_state_str[50] = { 0 };
    FS_BSTR signature_name;
    FS_UINT32 sig_state;
    FSLTVState ltv_state;
    FS_SIGNATUREVERIFYRESULT_HANDLE sig_verify_result = sig_verify_result_array[i];
    FSDK_BStr_Init(&signature_name);
    FSDK_SignatureVerifyResult_GetSignatureName(sig_verify_result, &signature_name);
    FSDK_SignatureVerifyResult_GetSignatureState(sig_verify_result, &sig_state);    
    FSDK_SignatureVerifyResult_GetLTVState(sig_verify_result, &ltv_state);    
    switch (ltv_state) {
    case e_FSLTVStateInactive:      
      sprintf_s(ltv_state_str, 50, "inactive");
      break;
    case e_FSLTVStateEnable:
      sprintf_s(ltv_state_str, 50, "enabled");
      break;
    case e_FSLTVStateNotEnable:
      sprintf_s(ltv_state_str, 50, "not enabled");
      break;
    }
    printf("Signature name:%s, signature state: %s, LTV state: %s\r\n", signature_name.str, TransformSignatureStateToString(sig_str, sig_size, sig_state), ltv_state_str);
    FSDK_BStr_Clear(&signature_name);
    //signature_name.ReleaseBuffer();
  }

  if (is_to_add_dss) {
    size_t i = 0;
    for (i = 0; i < length; i++) {
      FS_UINT32 state;
      FSDK_SignatureVerifyResult_GetSignatureState(sig_verify_result_array[i], &state);
      if (state & e_FSStateVerifyValid)
        FSDK_LTVVerifier_AddDSS(ltv_verifier, sig_verify_result_array[i]);
    }
  }
  free(sig_verify_result_array);
}

void DoLTV(wchar_t* signed_pdf_path, const wchar_t* saved_ltv_pdf_path) {
  // Use default SignatureCallback for signing a time stamp signature with filter "Adobe.PPKLite" and subfilter "ETSI.RFC3161",
  // so no need to register a custom signature callback.
  FS_TIMESTAMPSERVER_HANDLE timestamp_server;
  FS_WSTR password;
  FS_PDFDOC_HANDLE pdf_doc;
  FSErrorCode code;
  FS_PROGRESSIVE_HANDLE progressive;
  FS_PDFPAGE_HANDLE pdf_page;
  FS_SIGNATURE_HANDLE timestamp_signature;
  FS_PDFDOC_HANDLE check_pdf_doc;
  FS_PROGRESSIVE_HANDLE sign_progressive;
  FS_PROGRESSIVE_HANDLE progressive1;
  int rate;
  FSRectF rect;


  FSDK_TimeStampServerMgr_Initialize();  
  password.str = NULL;
  password.len = 0;
  code = FSDK_TimeStampServerMgr_AddServer(FREE_ETSIRFC316TSA_SERVER_NAME, FREE_ETSIRFC316TSA_SERVER_URL, L"", &password, &timestamp_server);
  FSDK_TimeStampServerMgr_SetDefaultServer0(timestamp_server);

  FSDK_PDFDoc_Create0(signed_pdf_path, &pdf_doc);
  FSDK_PDFDoc_StartLoad(pdf_doc, NULL, TRUE, NULL, &progressive);
  // Add DSS
  printf("== Before Add DSS ==\r\n");
  UseLTVVerifier(pdf_doc, TRUE);

  // Add DTS
  FSDK_PDFDoc_GetPage(pdf_doc, 0, &pdf_page);

  // The new time stamp signature will have default filter name "Adobe.PPKLite" and default subfilter name "ETSI.RFC3161".
  rect.left = rect.bottom = rect.right = rect.top = 0;
  FSDK_PDFPage_AddSignature1(pdf_page, rect, L"", e_FSSignatureTypeTimeStamp, TRUE, &timestamp_signature);

  FSDK_Signature_StartSign(timestamp_signature, L"", L"", e_FSDigestSHA256, saved_ltv_pdf_path, NULL, NULL, &sign_progressive);
  FSDK_Progressive_GetRateOfProgress(sign_progressive, &rate);
  if (rate != 100) {
    FSState state;
    FSDK_Progressive_Continue(sign_progressive, &state);
  }
  // Check saved file.
  FSDK_PDFDoc_Create0(saved_ltv_pdf_path, &check_pdf_doc);
  FSDK_PDFDoc_StartLoad(check_pdf_doc, NULL, TRUE, NULL, &progressive1);
  // Just LTV veify.
  printf("== After Add DSS ==\r\n");
  UseLTVVerifier(check_pdf_doc, FALSE);

  FSDK_TimeStampServerMgr_Release();
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t file_name[MAX_FILE_PATH] = { 0 };
  wchar_t input_file_path[MAX_FILE_PATH] = { 0 };
  wchar_t signed_pdf_path[MAX_FILE_PATH] = { 0 };
  wchar_t saved_ltv_pdf_path[MAX_FILE_PATH] = { 0 };  
  FSErrorCode error_code;
 
  swprintf_s(output_directory, MAX_FILE_PATH, L"%sltv/", output_path);
  _wmkdir(output_directory);
  swprintf_s(input_file_path, MAX_FILE_PATH, L"%sAboutFoxit.pdf", input_path);
  // Initialize library
  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    return 1;
  }
  wprintf(L"Input file path: %s\r\n", input_file_path);
  {
    // Add a PKCS7 signature and sign it.      
    swprintf_s(signed_pdf_path, MAX_FILE_PATH, L"%sAboutFoxit_signed.pdf", output_directory);
    PKCS7Signature(input_file_path, signed_pdf_path);

    // Do LTV
    swprintf_s(saved_ltv_pdf_path, MAX_FILE_PATH, L"%sAboutFoxit_signed_ltv.pdf", output_directory);
    DoLTV(signed_pdf_path, saved_ltv_pdf_path);
  }

  return err_ret;
}

