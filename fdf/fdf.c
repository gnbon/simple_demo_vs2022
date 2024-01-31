// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to import annotations from FDF/XFDF files
// and export annotations to FDF/XFDF files.

#include<direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_fdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_annot_c.h"
#include "../../../include/fs_pdfform_c.h"

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
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  FSErrorCode error_code = e_FSErrSuccess;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsfdf/", output_path);
  _wmkdir(output_directory);

  {
    {
      // Import FDF file
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t fdf_file[MAX_FILE_PATH] = { 0 };
      FS_PDFDOC_HANDLE pdf_doc = NULL;
      FS_FDFDOC_HANDLE fdf_doc = NULL;
      FS_RANGE_HANDLE page_range = NULL;
      FS_BOOL return_result;
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FS_BOOL result_value;

      swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
      swprintf_s(fdf_file, MAX_FILE_PATH, L"%lsAnnotationData.fdf", input_path);

      FSDK_PDFDoc_Create0(input_file, &pdf_doc);
      error_code = FSDK_PDFDoc_Load(pdf_doc, NULL);
      if (error_code != e_FSErrSuccess) {
        wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
        return 1;
      }

      FSDK_FDFDoc_Create1(fdf_file, &fdf_doc);
      FSDK_Range_Create(&page_range);
      FSDK_PDFDoc_ImportFromFDF(pdf_doc, fdf_doc, e_FSAnnots, page_range, &return_result);
      printf("Import annotations from fdf with file.\n");

      swprintf_s(output_file, MAX_FILE_PATH, L"%lsAboutFoxit_importFDF.pdf", output_directory);

      FSDK_PDFDoc_SaveAs(pdf_doc, output_file, e_FSSaveFlagNoOriginal, &result_value);
      FSDK_Range_Release(page_range);
      FSDK_FDFDoc_Release(fdf_doc);
      FSDK_PDFDoc_Release(pdf_doc);
    }

    {
      // Import FDF file
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t fdf_file[MAX_FILE_PATH] = { 0 };
      FS_PDFDOC_HANDLE pdf_doc = NULL;
      FILE* file = NULL;
      FS_FDFDOC_HANDLE fdf_doc = NULL;
      FS_RANGE_HANDLE page_range = NULL;
      FS_BOOL return_result;
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
	  size_t file_size = 0;
	  char* buffer = NULL;

      swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);

      swprintf_s(fdf_file, MAX_FILE_PATH, L"%lsAnnotationData.fdf", input_path);

      FSDK_PDFDoc_Create0(input_file, &pdf_doc);
      error_code = FSDK_PDFDoc_Load(pdf_doc, NULL);
      if (error_code != e_FSErrSuccess) {
        wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
        return 1;
      }

      _wfopen_s(&file, fdf_file, L"rb+");

      fseek(file, 0, SEEK_END);
      file_size = (size_t)ftell(file);
      buffer = (char*)malloc(file_size * sizeof(char));
      memset(buffer, 0, file_size);

      fseek(file, 0, SEEK_SET);
      fread(buffer, sizeof(char), file_size, file);
      fclose(file);

      FSDK_FDFDoc_Create2(buffer, file_size, &fdf_doc);

      FSDK_Range_Create(&page_range);

      FSDK_PDFDoc_ImportFromFDF(pdf_doc, fdf_doc, e_FSAnnots, page_range, &return_result);
      printf("Import annotations from fdf with memory buffer.\n");

      swprintf_s(output_file, MAX_FILE_PATH, L"%lsAboutFoxit_importFDF_FromMemory.pdf", output_directory);
      FSDK_PDFDoc_SaveAs(pdf_doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
      free(buffer);
      FSDK_Range_Release(page_range);
      FSDK_FDFDoc_Release(fdf_doc);
      FSDK_PDFDoc_Release(pdf_doc);
    }

    {
      // Export FDF file
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_fdf[MAX_FILE_PATH] = { 0 };
      FS_PDFDOC_HANDLE pdf_doc = NULL;
      FS_FDFDOC_HANDLE fdf_doc = NULL;
      FS_RANGE_HANDLE page_range = NULL;
      FS_BOOL return_result;

      swprintf_s(input_file, MAX_FILE_PATH, L"%lsAnnotationDataExport.pdf", input_path);

      swprintf_s(output_fdf, MAX_FILE_PATH, L"%lsAnnotationDataExport_fdf.fdf", output_directory);

      FSDK_PDFDoc_Create0(input_file, &pdf_doc);
      error_code = FSDK_PDFDoc_Load(pdf_doc, NULL);
      if (error_code != e_FSErrSuccess) {
        wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
        return 1;
      }

      FSDK_FDFDoc_Create0(e_FSFDF, &fdf_doc);

      FSDK_Range_Create(&page_range);
      FSDK_PDFDoc_ExportToFDF(pdf_doc, fdf_doc, e_FSAnnots, page_range, &return_result);
      FSDK_FDFDoc_SaveAs(fdf_doc, output_fdf, &return_result);
      printf("Export a fdf file.\n");
      FSDK_Range_Release(page_range);
      FSDK_FDFDoc_Release(fdf_doc);
      FSDK_PDFDoc_Release(pdf_doc);
    }
    {
      // Export XFDF file
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_xfdf[MAX_FILE_PATH] = { 0 };
      FS_PDFDOC_HANDLE pdf_doc = NULL;
      FS_FDFDOC_HANDLE fdf_doc = NULL;
      FS_RANGE_HANDLE page_range = NULL;
      FS_BOOL return_result;

      swprintf_s(input_file, MAX_FILE_PATH, L"%lsAnnotationDataExport.pdf", input_path);
      swprintf_s(output_xfdf, MAX_FILE_PATH, L"%lsAnnotationDataExport_xfdf.xfdf", output_directory);

      FSDK_PDFDoc_Create0(input_file, &pdf_doc);
      error_code = FSDK_PDFDoc_Load(pdf_doc, NULL);
      if (error_code != e_FSErrSuccess) {
        wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
        return 1;
      }

      FSDK_FDFDoc_Create0(e_FSXFDF, &fdf_doc);

      FSDK_Range_Create(&page_range);

      FSDK_PDFDoc_ExportToFDF(pdf_doc, fdf_doc, e_FSAnnots, page_range, &return_result);
      FSDK_FDFDoc_SaveAs(fdf_doc, output_xfdf, &return_result);
      printf("Export an xfdf file.\n");

      FSDK_Range_Release(page_range);
      FSDK_FDFDoc_Release(fdf_doc);
      FSDK_PDFDoc_Release(pdf_doc);
    }
  }
  FSDK_Library_Release();
  return err_ret;
}
