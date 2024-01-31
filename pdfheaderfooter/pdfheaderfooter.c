﻿// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to add, delete and modify
// the header-footer of PDF file.

#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_headerfooter_c.h"

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
  FS_PDFDOC_HANDLE doc = NULL;
  FSHeaderFooter headerfooter;
  FS_HEADERFOOTERCONTENTGENERATOR_HANDLE contentgenerator = NULL;
  FS_WSTR return_generatecontent;
  wchar_t output_file[MAX_FILE_PATH] = { 0 };
  FS_BOOL return_result = TRUE;
  int page_count;
  FS_BOOL return_removeall_headerfooters;
  FSHeaderFooterContent content;
  FSPageNumberRange page_number_range;
  FSRectF rectF;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdfheaderfooter/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsHeaderFooter.pdf", input_path);

  // Load PDF document.
  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  FSDK_WStr_Init(&headerfooter.content.footer_center_content);
  FSDK_WStr_Init(&headerfooter.content.footer_left_content);
  FSDK_WStr_Init(&headerfooter.content.footer_right_content);
  FSDK_WStr_Init(&headerfooter.content.header_center_content);
  FSDK_WStr_Init(&headerfooter.content.header_left_content);
  FSDK_WStr_Init(&headerfooter.content.header_right_content);
  FSDK_PDFDoc_GetEditableHeaderFooter(doc, &headerfooter);

  // Update header-footer
  FSDK_HeaderFooterContentGenerator_Create(&contentgenerator);
  FSDK_HeaderFooterContentGenerator_AddDate(contentgenerator, e_FSDateFormatMSlashDSlashYYYY);
  FSDK_HeaderFooterContentGenerator_AddString(contentgenerator, L" foxit");
  content = headerfooter.content;

  FSDK_WStr_Init(&return_generatecontent);
  FSDK_HeaderFooterContentGenerator_GenerateContent(contentgenerator, &return_generatecontent);
  content.header_right_content = return_generatecontent;
  headerfooter.content = content;
  FSDK_PDFDoc_UpdateHeaderFooter(doc, headerfooter);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsHeaderFooter_UpdateHeaderFooter.pdf", output_directory);

  error_code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }
  printf("Update PDF document header-footer.\n");

  // Add header-footer
  headerfooter.text_color = 0x00FFFF;

  FSDK_PDFDoc_GetPageCount(doc, &page_count);
  page_number_range.end_number = page_count;
  page_number_range.filter = e_FSOdd;
  page_number_range.start_number = 1;
  headerfooter.page_range = page_number_range;
  headerfooter.has_fixedsize_for_print = FALSE;
  headerfooter.has_text_shrinked = FALSE;
  headerfooter.start_page_number = 2;

  rectF.left = 36;
  rectF.bottom = 70;
  rectF.right = 36;
  rectF.top = 70;
  headerfooter.page_margin = rectF;
  headerfooter.text_size = 8;
  FSDK_PDFDoc_AddHeaderFooter(doc, headerfooter);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsHeaderFooter_AddHeaderFooter.pdf", output_directory);
  error_code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }
  printf("Add PDF document header-footer.\n");

  // Remove all header-footers
  FSDK_PDFDoc_RemoveAllHeaderFooters(doc, &return_removeall_headerfooters);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsHeaderFooter_RemoveAll.pdf", output_directory);
  error_code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }
  printf("Remove PDF document all header-footers.\n");

  FSDK_HeaderFooterContentGenerator_Release(contentgenerator);
  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();
  printf("pdfheaderfooter test.\n");

  return err_ret;
}

