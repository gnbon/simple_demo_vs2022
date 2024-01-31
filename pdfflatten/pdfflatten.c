// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to flatten PDF pages.

#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"

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

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  wchar_t input_file[MAX_FILE_PATH] = { 0 };

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdfflatten/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsannotations.pdf", input_path);

  {
    // Load PDF document.
    FS_PDFDOC_HANDLE doc = NULL;
    FSErrorCode code = e_FSErrSuccess;
    int page_count = 0, i = 0;
    wchar_t flatten_all[MAX_FILE_PATH] = { 0 };
    FS_BOOL return_result = FALSE;

	code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (code != e_FSErrSuccess) return 1;
    code = FSDK_PDFDoc_Load(doc, NULL);
    if (code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
      return 1;
    }
    // Get page count.
    FSDK_PDFDoc_GetPageCount(doc, &page_count);
    // Flatten PDF pages.
    for (i = 0; i < page_count; i++) {
      FS_PDFPAGE_HANDLE page = NULL;
      FS_PROGRESSIVE_HANDLE return_value;
      FS_BOOL return_flatten;
      FSDK_PDFDoc_GetPage(doc, i, &page);
      // Parse page before flattening the page.
      FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);

      FSDK_PDFPage_Flatten(page, TRUE, e_FSFlattenAll, &return_flatten);
      FSDK_PDFPage_Release(page);
    }

    swprintf_s(flatten_all, MAX_FILE_PATH, L"%lsannotations_flatten_all.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, flatten_all, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    FSDK_PDFDoc_Release(doc);
  }

  {
    // Flatten PDF with no annotation
    FS_PDFDOC_HANDLE doc = NULL;
    FSErrorCode error_code = e_FSErrSuccess;
    int page_count = 0, i = 0;
    wchar_t flatten_noannot[MAX_FILE_PATH] = { 0 };
    FS_BOOL return_result = FALSE;

	error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
      return 1;
    }
    // Get page count.
    FSDK_PDFDoc_GetPageCount(doc, &page_count);
    // Flatten PDF pages.
    for (i = 0; i < page_count; i++) {
      FS_PDFPAGE_HANDLE page = NULL;
      FS_BOOL return_flatten;
      FS_PROGRESSIVE_HANDLE return_value = NULL;

      FSDK_PDFDoc_GetPage(doc, i, &page);
      // Parse page before flattening the page.

      FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);

      FSDK_PDFPage_Flatten(page, TRUE, e_FSFlattenNoAnnot, &return_flatten);
      FSDK_PDFPage_Release(page);
    }

    swprintf_s(flatten_noannot, MAX_FILE_PATH, L"%lsannotations_flatten_noannot.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, flatten_noannot, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    FSDK_PDFDoc_Release(doc);
  }
  {
    // Flatten PDF with no form control
    FS_PDFDOC_HANDLE doc = NULL;
    int page_count = 0, i = 0;
    wchar_t flatten_noformcontrol[MAX_FILE_PATH] = { 0 };
    FS_BOOL return_result = FALSE;

    FSErrorCode error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
      return 1;
    }
    // Get page count.
    FSDK_PDFDoc_GetPageCount(doc, &page_count);
    // Flatten PDF pages.
    for (i = 0; i < page_count; i++) {
      FS_PDFPAGE_HANDLE page = NULL;
      FS_PROGRESSIVE_HANDLE return_value = NULL;
      FS_BOOL return_flatten = FALSE;

      FSDK_PDFDoc_GetPage(doc, i, &page);
      // Parse page before flattening the page.

      FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);

      FSDK_PDFPage_Flatten(page, TRUE, e_FSFlattenNoFormControl, &return_flatten);
      FSDK_PDFPage_Release(page);
    }

    swprintf_s(flatten_noformcontrol, MAX_FILE_PATH, L"%lsannotations_flatten_noformcontrol.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, flatten_noformcontrol, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    FSDK_PDFDoc_Release(doc);
  }
  {
    // Flatten PDF with no form control and no annotation -- equals to nothing to be flattened.
    FS_PDFDOC_HANDLE doc = NULL;
    int page_count, i = 0;
    wchar_t flatten_noannotnoform[MAX_FILE_PATH] = { 0 };
    FS_BOOL return_result = FALSE;
    FSErrorCode error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
      return 1;
    }
    //Get page count.
    FSDK_PDFDoc_GetPageCount(doc, &page_count);
    // Flatten PDF pages.
    for (i = 0; i < page_count; i++) {
      FS_PDFPAGE_HANDLE page = NULL;
      FS_PROGRESSIVE_HANDLE return_value = NULL;
      FS_BOOL return_flatten;
      FSErrorCode code = FSDK_PDFDoc_GetPage(doc, i, &page);
      // Parse page before flattening the page.
      FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);
      FSDK_PDFPage_Flatten(page, TRUE, e_FSFlattenNoAnnot | e_FSFlattenNoFormControl, &return_flatten);
      FSDK_PDFPage_Release(page);
    }

    swprintf_s(flatten_noannotnoform, MAX_FILE_PATH, L"%lsannotations_flatten_noannotnoform.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, flatten_noannotnoform, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    FSDK_PDFDoc_Release(doc);
  }
  FSDK_Library_Release();

  printf("Flatten PDF demo done.\n");
  return err_ret;
}

