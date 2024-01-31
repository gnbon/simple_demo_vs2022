// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to get/set page label information
// in a PDF document.
#include <time.h>
#include<direct.h>
#include<stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfgraphicsobject_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_pdfpagelabel_c.h"

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
#define MAX_FILE_BUFFER 1024

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

wchar_t* StyleCodeToString(FSPDFPageLabelStyle style) {
  switch (style) {
  case e_FSNone: {
    return L"None";
  }
  case e_FSDecimalNums: {
    return L"DecimalNums";
  }
  case e_FSUpperRomanNums: {
    return L"UpperRomanNums";
  }
  case e_FSLowerRomanNums: {
    return L"LowerRomanNums";
  }
  case e_FSUpperLetters: {
    return L"UpperLetters";
  }
  case e_FSLowerLetters: {
    return L"LowerLetters";
  }
  }
  return L"";
}

void WriteUnicode(FILE* file, const wchar_t* text_content) {
  FS_WSTR str;
  FS_BSTR dst_bstr;

  str.str = (wchar_t*)text_content;
  str.len = wcslen(text_content);
  FSDK_WStr_UTF16LEEncode(&str, &dst_bstr);

  if (dst_bstr.len == 0) return;
  fwrite((const char*)dst_bstr.str, sizeof(char), dst_bstr.len, file);
}

void ShowPageLabelsInfo(FS_PDFDOC_HANDLE doc, const wchar_t* text_doc_path) {
  FS_PAGELABELS_HANDLE page_labels;
  int count, i = 0;
  FS_WSTR return_title;
  FS_WSTR return_prefix;
  FSPDFPageLabelStyle return_style;
  FILE* file = NULL;

  _wfopen_s(&file, text_doc_path, L"w+b");

  FSDK_PageLabels_Create(doc, &page_labels);
  FSDK_PDFDoc_GetPageCount(doc, &count);

  for (i = 0; i < count; i++) {
    wchar_t file_buffer[MAX_FILE_BUFFER];
    FSDK_PageLabels_GetPageLabelStyle(page_labels, i, &return_style);
    FSDK_WStr_Init(&return_title);
    FSDK_PageLabels_GetPageLabelTitle(page_labels, i, &return_title);
    FSDK_WStr_Init(&return_prefix);
    FSDK_PageLabels_GetPageLabelPrefix(page_labels, i, &return_prefix);
    swprintf_s(file_buffer, MAX_FILE_BUFFER, L"page index: %d\tstyle: %ls\ttitle: %ls\tprefix: %ls\r\n", i,
      StyleCodeToString(return_style), return_title.str, return_prefix.str);
    WriteUnicode(file, file_buffer);
  }

  FSDK_PageLabels_Release(page_labels);
  fclose(file);
}

int main(int argc, char *argv[]) {
  FSErrorCode error_code;
  int err_ret = 0;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t input_file[MAX_FILE_PATH];
  wchar_t text_path[MAX_FILE_PATH];
  wchar_t text_path_new[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE doc = NULL;
  FS_PAGELABELS_HANDLE page_labels = NULL;
  wchar_t pdf_removeall[MAX_FILE_PATH];
  FS_BOOL return_result = FALSE;
  int count;
  wchar_t pdf_new[MAX_FILE_PATH];

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspage_labels/", output_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }
  swprintf_s(input_file, MAX_FILE_PATH, L"%lspage_labels.pdf", input_path);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }
  swprintf_s(text_path, MAX_FILE_PATH, L"%lspage_labels_original_page_labels_info.txt", output_directory);

  ShowPageLabelsInfo(doc, text_path);
  // Add new page labels

  FSDK_PageLabels_Create(doc, &page_labels);
  FSDK_PageLabels_RemoveAll(page_labels);
  swprintf_s(pdf_removeall, MAX_FILE_PATH, L"%lsremoveall_page_labels.pdf", output_directory);

  error_code = FSDK_PDFDoc_SaveAs(doc, pdf_removeall, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }

  FSDK_PDFDoc_GetPageCount(doc, &count);
  if (count > 0) {
    FSDK_PageLabels_SetPageLabel(page_labels, 0, e_FSUpperRomanNums, 1, L"Cover-");
  }
  if (count > 1) {
    FSDK_PageLabels_SetPageLabel(page_labels, 1, e_FSLowerRomanNums, 1, L"Catolog-");
  }
  if (count > 2) {
    FSDK_PageLabels_SetPageLabel(page_labels, 2, e_FSDecimalNums, 1, L"Contents-");
  }
  if (count > 5) {
    FSDK_PageLabels_SetPageLabel(page_labels, count - 1, e_FSUpperLetters, 1, L"Appendix-");
  }

  swprintf_s(text_path_new, MAX_FILE_PATH, L"%lspage_labels_new_page_labels_info.txt", output_directory);

  ShowPageLabelsInfo(doc, text_path_new);
  swprintf_s(pdf_new, MAX_FILE_PATH, L"%lsnew_page_labels.pdf", output_directory);

  error_code = FSDK_PDFDoc_SaveAs(doc, pdf_new, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }

  FSDK_PageLabels_Release(page_labels);
  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();
  printf("Page label test.\n");

  return err_ret;
}

