// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to do OCR for a PDF page or PDF document.

#if (defined(_WIN32) || defined(_WIN64))

#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_ocr_c.h"

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
  wchar_t output_directory[MAX_FILE_PATH];
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH];
  // "ocr_resource_path" is the path of ocr resources. Please refer to Developer Guide for more details.
  const wchar_t* ocr_resource_path = L"";

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsocr/", output_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsocr/AboutFoxit_ocr.pdf", input_path);

  if (ocr_resource_path == L"") {
    printf("ocr_resource_path is still empty. Please set it with a valid path to OCR resource path.\n");
    return 1;
  }

  // Initialize OCR engine.
  error_code = FSDK_OCREngine_Initialize(ocr_resource_path);
  if (error_code != e_FSErrSuccess) {
    switch (error_code) {
    case e_FSErrInvalidLicense:
      printf("[Failed] OCR module is not contained in current Foxit PDF SDK keys.\n");
      break;
    default:
      printf("Fail to initialize OCR engine. Error: %d\n", error_code);
      break;
    }
    return 1;
  }
  printf("OCREngine is initialized.\n");

  // Set log for OCREngine. (This can be opened to set log file if necessary)
  //OCREngine::SetLogFile(output_directory+L"ocr.log");
  // Set languages.
  FSDK_OCREngine_SetLanguages(L"English");

  // OCR PDF document, only for search, not editable.
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_OCR_HANDLE ocr_operator;
    FS_BOOL is_editable = FALSE;
    wchar_t save_pdf_path[MAX_FILE_PATH];
    FS_BOOL return_result = FALSE;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    FSDK_OCR_Create(&ocr_operator);
    FSDK_OCR_OCRPDFDocument(ocr_operator, doc, is_editable);
    swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%lsocr_doc_searchable.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, save_pdf_path, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_OCR_Release(ocr_operator);
      return 1;
    }
    FSDK_OCR_Release(ocr_operator);
  }
  // OCR PDF document, editable.
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_OCR_HANDLE ocr_operator = NULL;
    FS_BOOL is_editable = TRUE;
    wchar_t save_pdf_path[MAX_FILE_PATH];
    FS_BOOL return_result = FALSE;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    FSDK_OCR_Create(&ocr_operator);
    FSDK_OCR_OCRPDFDocument(ocr_operator, doc, is_editable);

    swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%lsocr_doc_editable.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, save_pdf_path, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_OCR_Release(ocr_operator);
      return 1;
    }
    FSDK_OCR_Release(ocr_operator);
  }
  // OCR PDF page, only for search, not editable.
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    FS_OCR_HANDLE ocr_operator = NULL;
    FS_BOOL is_editable = FALSE;
    FS_BOOL return_result = FALSE;
    wchar_t save_pdf_path[MAX_FILE_PATH];

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    FSDK_OCR_Create(&ocr_operator);
    FSDK_OCR_OCRPDFDocument(ocr_operator, doc, is_editable);

    swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%lsocr_page_searchable.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, save_pdf_path, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_OCR_Release(ocr_operator);
      return 1;
    }
    FSDK_OCR_Release(ocr_operator);
  }
  // OCR PDF page, editable.
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    FS_OCR_HANDLE ocr_operator = NULL;
    FS_BOOL is_editable = TRUE;
    wchar_t save_pdf_path[MAX_FILE_PATH];
    FS_BOOL return_result = FALSE;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    FSDK_OCR_Create(&ocr_operator);
    FSDK_OCR_OCRPDFDocument(ocr_operator, doc, is_editable);

    swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%lsocr_page_editable.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, save_pdf_path, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_OCR_Release(ocr_operator);
      return 1;
    }
    FSDK_OCR_Release(ocr_operator);
  }

  FSDK_OCREngine_Release();
  FSDK_Library_Release();

  printf("END: OCR demo.\n");
  return 0;
}

#endif  // (defined(_WIN32) || defined(_WIN64))
