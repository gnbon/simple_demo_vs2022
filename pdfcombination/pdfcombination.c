// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to combine pdf files.
// Include Foxit SDK header files.

#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_combination_c.h"

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
  FS_COMBINEDOCUMENTINFO_HANDLE combine_document_array[3];
  FS_WSTR wstr;
  wchar_t pdfdocpath[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;
  wchar_t output_file[MAX_FILE_PATH] = { 0 };
  int i = 0;
  FS_PROGRESSIVE_HANDLE progressive = NULL;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdfcombination/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAnnot_all.pdf", input_path);

  printf("===Start combine pdf files===\n");

  wstr.str = L"";
  wstr.len = 0;
  FSDK_CombineDocumentInfo_Create(input_file, &wstr, &combine_document_array[0]);
  FSDK_CombineDocumentInfo_SetBookmarkTitle(combine_document_array[0], L"Annot_all.pdf");
  FSDK_CombineDocumentInfo_SetPDFFileName(combine_document_array[0], L"Annot_all.pdf");

  swprintf_s(pdfdocpath, MAX_FILE_PATH, L"%lsPDF2Img.pdf", input_path);

  FSDK_PDFDoc_Create0(pdfdocpath, &doc);
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (e_FSErrSuccess != error_code) {
    FSDK_PDFDoc_Release(doc);
    doc = NULL;
  }
  FSDK_CombineDocumentInfo_Create0(doc, &combine_document_array[1]);
  FSDK_CombineDocumentInfo_SetBookmarkTitle(combine_document_array[1], L"PDF2Img.pdf");
  FSDK_CombineDocumentInfo_SetPDFFileName(combine_document_array[1], L"PDF2Img.pdf");

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsSamplePDF.pdf", input_path);
  FSDK_CombineDocumentInfo_Create(input_file, &wstr, &combine_document_array[2]);
  FSDK_CombineDocumentInfo_SetBookmarkTitle(combine_document_array[2], L"SamplePDF.pdf");
  FSDK_CombineDocumentInfo_SetPDFFileName(combine_document_array[2], L"SamplePDF.pdf");

  swprintf_s(output_file, MAX_FILE_PATH, L"%lspdfcombination.pdf", output_directory);

  FSDK_Combination_StartCombineDocuments(output_file, combine_document_array, 3, 0xFFFFFFFC, NULL, &progressive);
  
  for (i = 0; i < 3; i++)
    FSDK_CombineDocumentInfo_Release(combine_document_array[i]);
  FSDK_Library_Release();
  printf("===PDF combination end===\n");

  return err_ret;
}
