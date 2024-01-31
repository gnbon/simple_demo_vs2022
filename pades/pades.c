// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to add, sign, verify and get PAdES level of
// a PAdES signature in PDF document.

// Include header files.
#include <locale.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
#include<direct.h>
#else
#include <sys/stat.h>
#endif

// Include Foxit SDK header files.
#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_signature_c.h"

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
#if defined(_WIN32) || defined(_WIN64)
static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";
#else
static const wchar_t* output_path = L"./output_files/";
static const wchar_t* input_path = L"./input_files/";
#endif



wchar_t output_directory[MAX_FILE_PATH] = { 0 };  // output_path + wstring(L"pades/");

#define TIMESTAMP_SIGNATURE_FILTER "Adobe.PPKLite"
#define TIMESTAMP_SIGNATURE_SUBFILTER "ETSI.RFC3161"
#define FREE_ETSIRFC316TSA_SERVER_NAME L"FreeTSAServer"
#define FREE_ETSIRFC316TSA_SERVER_URL L"http://ca.signfiles.com/TSAServer.aspx"

#define ETSICADES_SIGNATURE_FILTER "Adobe.PPKLite"
#define ETSICADES_SIGNATURE_SUBFILTER "ETSI.CAdES.detached"


#if !defined(WIN32) && !defined(_WIN64)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif

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

wchar_t* GenerateNewPDFFileName(wchar_t* new_file_name, size_t new_file_size, const wchar_t* org_file_name, const wchar_t* suffix_name) {
  size_t org_length = wcslen(org_file_name);
  wchar_t file_name[20] = { 0 };
  wcsncpy_s(file_name, 20, org_file_name, org_length - 4);
  swprintf_s(new_file_name, new_file_size, L"%s%s.pdf", file_name, suffix_name);

  return new_file_name;
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

const wchar_t* TransformLevel2WideString(wchar_t* str_level, size_t level_size, FSPAdESLevel level) {
  switch (level) {
  case e_FSPAdESLevelNotPAdES:
    swprintf_s(str_level, level_size, L"NotPades");
    break;
  case e_FSPAdESLevelNone:
    swprintf_s(str_level, level_size, L"NoneLevel");
    break;
  case e_FSPAdESLevelBB:
    swprintf_s(str_level, level_size, L"LevelB");
    break;
  case e_FSPAdESLevelBT:
    swprintf_s(str_level, level_size, L"LevelT");
    break;
  case e_FSPAdESLevelBLT:
    swprintf_s(str_level, level_size, L"LevelLT");
    break;
  case e_FSPAdESLevelBLTA:
    swprintf_s(str_level, level_size, L"LevelLTA");
    break;
  default:
    swprintf_s(str_level, level_size, L"Unknown level value");
    break;
  }
  return str_level;
}

const char* TransformLevel2String(char* str_level, size_t level_size, FSPAdESLevel level) {
  switch (level) {
  case e_FSPAdESLevelNotPAdES:
    sprintf_s(str_level, level_size, "NotPades");
    break;
  case e_FSPAdESLevelNone:
    sprintf_s(str_level, level_size, "NoneLeve");
    break;
  case e_FSPAdESLevelBB:
    sprintf_s(str_level, level_size, "LevelB");
    break;
  case e_FSPAdESLevelBT:
    sprintf_s(str_level, level_size, "LevelT");
    break;
  case e_FSPAdESLevelBLT:
    sprintf_s(str_level, level_size, "LevelLT");
    break;
  case e_FSPAdESLevelBLTA:
    sprintf_s(str_level, level_size, "LevelLTA");
    break;
  default:
    sprintf_s(str_level, level_size, "Unknown level value");
    break;
  }
  return str_level;
}

void PAdESSign(wchar_t* input_pdf_path, wchar_t* signed_pdf_path, FSPAdESLevel pades_level) {
  FS_PDFPAGE_HANDLE pdf_page;
  float page_width = 0.00;
  float page_height = 0.00;
  FS_PDFDOC_HANDLE pdf_doc;
  FSErrorCode error_code;
  FSRectF new_sig_rect;
  FSDateTime sign_time;
  FS_SIGNATURE_HANDLE new_signature;
  char new_value[200];
  size_t new_value_size = 200;
  wchar_t cert_path[MAX_FILE_PATH] = { 0 };
  wchar_t cached_signed_pdf_path[MAX_FILE_PATH] = { 0 };
  wchar_t sub_signed_pdf_path[MAX_FILE_PATH] = { 0 };
  wchar_t* real_signed_pdf_path = signed_pdf_path;
  const wchar_t* cert_password = L"123456";
  FS_PROGRESSIVE_HANDLE sign_progressive;
  FS_UINT32 ap_flags;
  int return_value;
  char level_str[100] = { 0 };
  size_t level_size = 100;
  wchar_t w_new_value[200] = { 0 };
  size_t w_new_value_size = 200;

  printf("To add a PAdES signature in %s\r\n", TransformLevel2String(level_str, level_size, pades_level));
  
  wmemset(cached_signed_pdf_path, 0, MAX_FILE_PATH);
  if (pades_level > e_FSPAdESLevelBT) {
    wcsncpy_s(sub_signed_pdf_path, MAX_FILE_PATH, signed_pdf_path, wcslen(signed_pdf_path) - 4);
    swprintf_s(cached_signed_pdf_path, MAX_FILE_PATH, L"%s_cache.pdf", sub_signed_pdf_path);    
  } else {
    swprintf_s(cached_signed_pdf_path, MAX_FILE_PATH, L"%s", signed_pdf_path);
  }

  error_code = FSDK_PDFDoc_Create0(input_pdf_path, &pdf_doc);
  if (error_code != e_FSErrSuccess) return;
  error_code = FSDK_PDFDoc_Load(pdf_doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(pdf_doc);
    printf("FSDK_PDFDoc_Load error_code: %d\n", error_code);
    return;
  }  
  FSDK_PDFDoc_GetPage(pdf_doc, 0, &pdf_page);
  FSDK_PDFPage_GetWidth(pdf_page, &page_width);  
  FSDK_PDFPage_GetHeight(pdf_page, &page_height);
  new_sig_rect.left = page_width / 2 - 50.0f;
  new_sig_rect.bottom = page_height / 2 - 50.0f;
  new_sig_rect.right = page_width / 2 + 50.0f;
  new_sig_rect.top = page_height / 2 + 50.0f;
  FSDK_PDFPage_AddSignature(pdf_page, new_sig_rect, &new_signature);
  FSDK_Signature_SetFilter(new_signature, ETSICADES_SIGNATURE_FILTER);
  FSDK_Signature_SetSubFilter(new_signature, ETSICADES_SIGNATURE_SUBFILTER);

  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameSigner, L"Foxit PDF SDK");
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameContactInfo, L"support@foxitsoftware.com");
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameDN, L"CN=CN,MAIL=MAIL@MAIL.COM");
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameLocation, L"Fuzhou, China");

  memset(level_str, 0, level_size);
  sprintf_s(new_value, new_value_size, "As a sample for subfilter \"%s\", in %s ", ETSICADES_SIGNATURE_SUBFILTER, TransformLevel2String(level_str, level_size, pades_level));
  string2wstring(new_value, strlen(new_value), w_new_value, w_new_value_size);
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameReason, w_new_value);
  FSDK_Signature_SetKeyValue(new_signature, e_FSKeyNameText, w_new_value);
  sign_time = GetLocalDateTime();
  FSDK_Signature_SetSignTime(new_signature, sign_time);
  // Set appearance flags to decide which content would be used in appearance.
  ap_flags = e_FSAPFlagLabel | e_FSAPFlagSigner |
    e_FSAPFlagReason | e_FSAPFlagDN |
    e_FSAPFlagLocation | e_FSAPFlagText |
    e_FSAPFlagSigningTime;
  FSDK_Signature_SetAppearanceFlags(new_signature, ap_flags);

  swprintf_s(cert_path, MAX_FILE_PATH, L"%sfoxit_all.pfx", input_path);
  FSDK_Signature_StartSign(new_signature, cert_path, cert_password, e_FSDigestSHA256, cached_signed_pdf_path, NULL, NULL, &sign_progressive);
  FSDK_Progressive_GetRateOfProgress(sign_progressive, &return_value);
  if (return_value != 100) {
    FSState progress_state;
    FSDK_Progressive_Continue(sign_progressive, &progress_state);
    if (e_FSFinished != progress_state) {
      printf("[Failed] Fail to sign the new CAdES signature.\r\n");
      return;
    }
  }

  if (pades_level > e_FSPAdESLevelBT) {
    FS_PDFDOC_HANDLE cache_pdf_doc;
    error_code = FSDK_PDFDoc_Create0(cached_signed_pdf_path, &cache_pdf_doc);
    if (error_code != e_FSErrSuccess) return;
    error_code = FSDK_PDFDoc_Load(cache_pdf_doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(cache_pdf_doc);
      return;
    }
    // Here, we only simply create an empty DSS object in PDF document, just as a simple exmaple.
    // In fact, user should use LTVVerifier to add DSS.
    FSDK_PDFDoc_CreateDSS(cache_pdf_doc);

    if (pades_level > e_FSPAdESLevelBLT) {
      FS_PDFPAGE_HANDLE cache_pdf_page;
      FSRectF rect = {0, 0, 0, 0};
      FSErrorCode error_code;
      int return_value;
      FS_SIGNATURE_HANDLE time_stamp_signature;
      FS_PROGRESSIVE_HANDLE sign_ts_progressive = NULL;
      FSDK_PDFDoc_GetPage(cache_pdf_doc, 0, &cache_pdf_page);
      FSDK_PDFPage_AddSignature1(cache_pdf_page, rect, L"", e_FSSignatureTypeTimeStamp, TRUE, &time_stamp_signature);
      FSDK_Signature_StartSign(time_stamp_signature, L"", L"", e_FSDigestSHA256, real_signed_pdf_path, NULL, NULL, &sign_ts_progressive);
      error_code = FSDK_Progressive_GetRateOfProgress(sign_ts_progressive, &return_value);
      if (return_value != 100) {
        FSState progress_state;
        FSDK_Progressive_Continue(sign_ts_progressive, &progress_state);
        if (e_FSFinished != progress_state) {
          printf("[Failed] Fail to sign the new time stamp signature.\r\n");
          return;
        }
      }
      FSDK_PDFPage_Release(cache_pdf_page);
    } else {
      FS_BOOL return_result = FALSE;
      error_code = FSDK_PDFDoc_SaveAs(cache_pdf_doc, real_signed_pdf_path, e_FSSaveFlagIncremental, &return_result);
      if (error_code != e_FSErrSuccess) {
        printf("Save doc error!\n");
        FSDK_PDFDoc_Release(cache_pdf_doc);
        return;
      }
    }
    FSDK_PDFDoc_Release(cache_pdf_doc);
  }
  FSDK_PDFPage_Release(pdf_page);
  FSDK_PDFDoc_Release(pdf_doc);
}

void PAdESVerify(const wchar_t* check_pdf_path, FSPAdESLevel expect_pades_level) {
  int i;
  int sig_count;
  FS_BOOL has_cades_signature;
  FS_PDFDOC_HANDLE check_pdf_doc;
  FSErrorCode error_code = FSDK_PDFDoc_Create0(check_pdf_path, &check_pdf_doc);
  if (error_code != e_FSErrSuccess) return;
  error_code = FSDK_PDFDoc_Load(check_pdf_doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(check_pdf_doc);
    return;
  }
  wprintf(L"To verify level of PAdES signature in file %s\r\n", check_pdf_path);
  FSDK_PDFDoc_GetSignatureCount(check_pdf_doc, &sig_count);
  if (0 == sig_count)
    printf("No signature in current PDF file.\r\n");

  has_cades_signature = FALSE;
  for (i = 0; i < sig_count; i++) {
    FS_BOOL is_true;
    FS_BSTR return_value;
    FS_UINT32 sig_org_state;
    FS_SIGNATURE_HANDLE temp_sig;
    FSDK_PDFDoc_GetSignature(check_pdf_doc, i, &temp_sig);
    if (!temp_sig) continue;    
    FSDK_Signature_GetState(temp_sig, &sig_org_state);
    is_true = sig_org_state & e_FSStateSigned;
    if (!is_true) continue;
    FSDK_BStr_Init(&return_value);
    FSDK_Signature_GetSubFilter(temp_sig, &return_value);
    if (strcmp(return_value.str, ETSICADES_SIGNATURE_SUBFILTER) == 0) {
      int iverify;
      FS_UINT32 sig_state;
      FSPAdESLevel actual_level;
      char sig_state_str[1000] = { 0 };
      size_t sig_state_size = 1000;
      char level_str[100] = { 0 };
      size_t level_size = 100;
      // Verify PAdES signature.
      FS_PROGRESSIVE_HANDLE verify_progressive = NULL;
      has_cades_signature = TRUE;
      FSDK_Signature_StartVerify(temp_sig, NULL, NULL, &verify_progressive);

      FSDK_Progressive_GetRateOfProgress(verify_progressive, &iverify);
      if (100 != iverify) {
        FSState progress_state;
        FSDK_Progressive_Continue(verify_progressive, &progress_state);
        if (e_FSFinished != progress_state) {
          printf("[Failed] Fail to verify a PAdES signature. Signature index:%d\r\n", i);
          continue;
        }
      }
      FSDK_Signature_GetState(temp_sig, &sig_state);
      printf("Signature index: %d, a PAdES signature. State after verifying: %s\r\n", i, TransformSignatureStateToString(sig_state_str, sig_state_size, sig_state));

      // Get PAdES level.
      FSDK_Signature_GetPAdESLevel(temp_sig, &actual_level);
      printf("Signature index:%d, PAdES level:%s, %s\r\n",
        i, TransformLevel2String(level_str, level_size, actual_level),
        actual_level == expect_pades_level ? "matching expected level." : "NOT match expected level.");
    }
  }
  if (FALSE == has_cades_signature)
    printf("No PAdES signature in current PDF file.\r\n");
  FSDK_PDFDoc_Release(check_pdf_doc);
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  const wchar_t* input_file_name = L"AboutFoxit.pdf";
  wchar_t input_file_path[MAX_FILE_PATH] = { 0 };
  wchar_t signed_pdf_path[MAX_FILE_PATH] = { 0 };
  FSErrorCode error_code;
  FSPAdESLevel level;
  FS_TIMESTAMPSERVER_HANDLE timestamp_server;
  FS_WSTR password;
  wchar_t level_str[100] = { 0 };
  size_t level_size = 100;
  wchar_t new_file_name[200] = { 0 };
  size_t new_file_size = 200;

  swprintf_s(output_directory, MAX_FILE_PATH, L"%spades/", output_path);
  _wmkdir(output_directory);    
  swprintf_s(input_file_path, MAX_FILE_PATH, L"%sAboutFoxit.pdf", input_path);
  // Initialize library
  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    return 1;
  }
  wprintf(L"Input file path: %s \n", input_file_path);
  FSDK_TimeStampServerMgr_Initialize();

  level = e_FSPAdESLevelBB;
  swprintf_s(input_file_path, MAX_FILE_PATH, L"%sAboutFoxit.pdf", input_path);
  swprintf_s(signed_pdf_path, MAX_FILE_PATH, L"%sAboutFoxitLevelB.pdf", output_directory);
  PAdESSign(input_file_path, signed_pdf_path, level);
  PAdESVerify(signed_pdf_path, level);

  password.str = NULL;
  password.len = 0;
  FSDK_TimeStampServerMgr_AddServer(FREE_ETSIRFC316TSA_SERVER_NAME, FREE_ETSIRFC316TSA_SERVER_URL, L"", &password, &timestamp_server);
  FSDK_TimeStampServerMgr_SetDefaultServer0(timestamp_server);
  level = e_FSPAdESLevelBT;
  while (TRUE) {
    swprintf_s(signed_pdf_path, MAX_FILE_PATH, L"%s%s", output_directory, 
      GenerateNewPDFFileName(new_file_name, new_file_size, input_file_name, TransformLevel2WideString(level_str, level_size, level)));
    
    PAdESSign(input_file_path, signed_pdf_path, level);
    PAdESVerify(signed_pdf_path, level);
    level = (FSPAdESLevel)(level + 1);
    if (level > e_FSPAdESLevelBLTA) break;
  }

  FSDK_TimeStampServerMgr_Release();

  return err_ret;
}

