// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to do output preview.


// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>
#include <windows.h>

// Include Foxit SDK header files.
#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fx_stream_c.h"
#include "../../../include/fx_basic_c.h"
#include "../../../include/fs_annot_c.h"
#include "../../../include/fs_image_c.h"
#include "../../../include/fs_render_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_outputpreview_c.h"

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

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  wchar_t input_file_path[MAX_FILE_PATH] = { 0 };

  // "default_icc_folder_path" is the path of the folder which contains default icc profile files. Please refer to Developer Guide for more details.
  const wchar_t* default_icc_folder_path = L"";

  FS_PDFDOC_HANDLE pdf_doc = NULL;
  FS_PROGRESSIVE_HANDLE progressive1 = NULL;
  FS_PDFPAGE_HANDLE pdf_page = NULL;
  FS_PROGRESSIVE_HANDLE progressive2 = NULL;
  float page_width;
  float page_height;
  int bitmap_width, bitmap_height = 0;
  FSDIBFormat bitmap_format = e_FSDIBInvalid;
  FS_UINT32 background_color = 0x000000;
  FS_BOOL result;
  FSMatrix display_matrix;
  FS_BITMAP_HANDLE render_bitmap = NULL;
  FS_RENDERER_HANDLE renderer = NULL;
  FS_OUTPUTPREVIEW_HANDLE output_preview = NULL;
  wchar_t simulation_icc_file_path[MAX_FILE_PATH] = { 0 };
  FS_UINT32 process_plates_length = 0;
  FS_UINT32 spot_plates_length = 0;
  FS_UINT32 i = 0;
  FS_BSTR* process_plates = NULL;
  FS_BSTR *spot_plates = NULL;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsoutput_preview/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file_path, MAX_FILE_PATH, L"%lspage_organization_123.pdf", input_path);
  wprintf(L"Input file path: %ls\n", input_file_path);

  if (wcslen(default_icc_folder_path) == 0) {
    printf("default_icc_folder_path is still empty. Please set it with a valid path to the folder which contains default icc profile files.\n");
    return 1;
  }
  // Set folder path which contains default icc profile files.
  error_code = FSDK_Library_SetDefaultICCProfilesPath(default_icc_folder_path);
  if (error_code != e_FSErrSuccess) {
    if (e_FSErrFilePathNotExist == error_code) {
      printf("default_icc_folder_path does not exist. Please set it with a valid path to the folder which contains default icc profile files.\n");
      return 1;
    }
    else {
      printf("error: %d\n", error_code);
      return 1;
    }
  }

  FSDK_PDFDoc_Create0(input_file_path, &pdf_doc);
  FSDK_PDFDoc_StartLoad(pdf_doc, NULL, TRUE, NULL, &progressive1);
  FSDK_PDFDoc_GetPage(pdf_doc, 0, &pdf_page);

  FSDK_PDFPage_StartParse(pdf_page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive2);
  FSDK_PDFPage_GetWidth(pdf_page, &page_width);
  FSDK_PDFPage_GetHeight(pdf_page, &page_height);
  bitmap_width = (int)page_width;
  bitmap_height = (int)page_height;

  FSDK_PDFPage_HasTransparency(pdf_page, &result);
  if (result) {
    background_color = 0x000000;
    bitmap_format = e_FSDIBArgb;
  }
  else {
    background_color = 0xFFFFFF;
    bitmap_format = e_FSDIBRgb32;
  }

  FSDK_PDFPage_GetDisplayMatrix(pdf_page, 0, 0, bitmap_width, bitmap_height, e_FSRotation0, &display_matrix);

  FSDK_Bitmap_Create(bitmap_width, bitmap_height, bitmap_format, NULL, 0, &render_bitmap);
  FSDK_Bitmap_FillRect(render_bitmap, background_color, NULL);

  FSDK_Renderer_Create(render_bitmap, FALSE, &renderer);

  FSDK_OutputPreview_Create(pdf_doc, &output_preview);

  swprintf_s(simulation_icc_file_path, MAX_FILE_PATH, L"%lsicc_profile/USWebCoatedSWOP.icc", input_path);

  FSDK_OutputPreview_SetSimulationProfile(output_preview, simulation_icc_file_path);
  FSDK_OutputPreview_SetShowType(output_preview, e_FSShowAll);

  FSDK_OutputPreview_GetPlates(output_preview, e_FSColorantTypeProcess, NULL, &process_plates_length);
  
  process_plates = (FS_BSTR*)malloc(sizeof(FS_BSTR) * process_plates_length);
  for (i = 0; i < process_plates_length; i++)
    FSDK_BStr_Init(&process_plates[i]);
  FSDK_OutputPreview_GetPlates(output_preview, e_FSColorantTypeProcess, process_plates, &process_plates_length);

  FSDK_OutputPreview_GetPlates(output_preview, e_FSColorantTypeSpot, NULL, &spot_plates_length);
  spot_plates = (FS_BSTR*)malloc(sizeof(FS_BSTR) * spot_plates_length);
  for (i = 0; i < spot_plates_length; i++)
    FSDK_BStr_Init(&spot_plates[i]);
  FSDK_OutputPreview_GetPlates(output_preview, e_FSColorantTypeSpot, spot_plates, &spot_plates_length);

  // Set check status of spot plate to be true, if there's any spot plate.
  for (i = 0; i < spot_plates_length; i++) {
    error_code = FSDK_OutputPreview_SetCheckStatus(output_preview, spot_plates[i].str, TRUE);
  }

  for (i = 0; i < spot_plates_length; i++)
    FSDK_BStr_Clear(&spot_plates[i]);
  free(spot_plates);

  // Only set one process plate to be checked each time and generate the preview bitmap.
  for (i = 0; i < (int)process_plates_length; i++) {
	FS_BITMAP_HANDLE preview_bitmap = NULL;
    FS_IMAGE_HANDLE result_image = NULL;
    FS_BOOL is_add = FALSE;
    wchar_t saved_file_path[MAX_FILE_PATH] = { 0 };
    FS_BOOL result_value;

    if (0 != i)
      error_code = FSDK_OutputPreview_SetCheckStatus(output_preview, process_plates[i - 1].str, FALSE);
    error_code = FSDK_OutputPreview_SetCheckStatus(output_preview, process_plates[i].str, TRUE);

    error_code = FSDK_OutputPreview_GeneratePreviewBitmap(output_preview, pdf_page, display_matrix, renderer, &preview_bitmap);

    FSDK_Image_Create(&result_image);

    error_code = FSDK_Image_AddFrame(result_image, preview_bitmap, &is_add);

    swprintf_s(saved_file_path, MAX_FILE_PATH, L"%lspreview_result_ProcessPlate[%d]_true.bmp", output_directory, i);

    FSDK_Image_SaveAs(result_image, saved_file_path, &result_value);
    FSDK_Image_Release(result_image);
  }

  for (i = 0; i < process_plates_length; i++)
    FSDK_BStr_Clear(&process_plates[i]);
  free(process_plates);

  FSDK_OutputPreview_Release(output_preview);
  FSDK_Renderer_Release(renderer);
  FSDK_Bitmap_Release(render_bitmap);
  FSDK_PDFPage_Release(pdf_page);
  FSDK_PDFDoc_Release(pdf_doc);
  FSDK_Library_Release();
  printf("[END] demo output_preview.\n");

  return err_ret;
}

