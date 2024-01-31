// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to add watermarks (in different types)
// into PDF files.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_watermark_c.h"
#include "../../../include/fs_image_c.h"

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

void AddTextWatermark(FS_PDFDOC_HANDLE doc, FS_PDFPAGE_HANDLE page) {
  FSWatermarkSettings settings;
  FSWatermarkTextProperties text_properties;
  FS_FONT_HANDLE font = NULL;
  FS_WATERMARK_HANDLE watermark = NULL;
  FS_BOOL return_value;

  settings.flags = e_FSWatermarkFlagsFlagASPageContents | e_FSWatermarkFlagsFlagOnTop;
  settings.offset_x = 0;
  settings.offset_y = 0;
  settings.opacity = 90;
  settings.position = e_FSPosTopRight;
  settings.rotation = -45.f;
  settings.scale_x = 1.f;
  settings.scale_y = 1.f;

  text_properties.alignment = e_FSAlignmentCenter;
  text_properties.color = 0xF68C21;
  text_properties.font_style = e_FSFontStyleFontStyleNormal;
  text_properties.line_space = 1;
  text_properties.font_size = 12.f;

  FSDK_Font_Create0(e_FSStdIDTimesB, &font);
  text_properties.font = font;

  FSDK_Watermark_Create(doc, L"Foxit PDF SDK\nwww.foxitsoftware.com", text_properties, settings, &watermark);
  FSDK_Watermark_InsertToPage(watermark, page, &return_value);
  FSDK_Watermark_Release(watermark);
}

void AddBitmapWatermark(FS_PDFDOC_HANDLE doc, FS_PDFPAGE_HANDLE page, const wchar_t* bitmap_file) {
  FSWatermarkSettings settings;
  FS_IMAGE_HANDLE image = NULL;
  FS_BITMAP_HANDLE bitmap = NULL;
  float fwidth = 0.0f;
  int page_width = 0;
  float height;
  int iwidth;
  FS_WATERMARK_HANDLE watermark = NULL;
  FS_BOOL return_value;

  settings.flags = e_FSWatermarkFlagsFlagASPageContents | e_FSWatermarkFlagsFlagOnTop;
  settings.offset_x = 0.f;
  settings.offset_y = 0.f;
  settings.opacity = 60;
  settings.position = e_FSPosCenterLeft;
  settings.rotation = 90.f;

  FSDK_Image_Create0(bitmap_file, &image);
  FSDK_Image_GetFrameBitmap(image, 0, &bitmap);
  FSDK_PDFPage_GetWidth(page, &fwidth);
  page_width = (int)fwidth;

  FSDK_PDFPage_GetHeight(page, &height);

  FSDK_Bitmap_GetWidth(bitmap, &iwidth);
  settings.scale_x = height * 1.0f / iwidth;
  settings.scale_y = settings.scale_x;

  FSDK_Watermark_Create0(doc, bitmap, settings, &watermark);
  FSDK_Watermark_InsertToPage(watermark, page, &return_value);
  FSDK_Bitmap_Release(bitmap);
  FSDK_Image_Release(image);
  FSDK_Watermark_Release(watermark);
}

void AddImageWatermark(FS_PDFDOC_HANDLE doc, FS_PDFPAGE_HANDLE page, const wchar_t* image_file) {
  FSWatermarkSettings settings;
  FS_IMAGE_HANDLE image = NULL;
  FS_BITMAP_HANDLE bitmap = NULL;
  float fwidth;
  int page_width = 0;
  float fheight;
  int iwidth;
  FS_WATERMARK_HANDLE watermark = NULL;
  FS_BOOL return_value;

  settings.flags = e_FSWatermarkFlagsFlagASPageContents | e_FSWatermarkFlagsFlagOnTop;
  settings.offset_x = 0.f;
  settings.offset_y = 0.f;
  settings.opacity = 20;
  settings.position = e_FSPosCenter;
  settings.rotation = 0.0f;

  FSDK_Image_Create0(image_file, &image);
  FSDK_Image_GetFrameBitmap(image, 0, &bitmap);
  FSDK_PDFPage_GetWidth(page, &fwidth);

  FSDK_PDFPage_GetHeight(page, &fheight);
  page_width = (int)fwidth;

  FSDK_Bitmap_GetWidth(bitmap, &iwidth);
  settings.scale_x = fwidth * 0.618f / iwidth;
  settings.scale_y = settings.scale_x;

  FSDK_Watermark_Create1(doc, image, 0, settings, &watermark);

  FSDK_Watermark_InsertToPage(watermark, page, &return_value);
  FSDK_Bitmap_Release(bitmap);
  FSDK_Image_Release(image);
  FSDK_Watermark_Release(watermark);
}

void AddSingleWatermark(FS_PDFDOC_HANDLE doc, FS_PDFPAGE_HANDLE page) {
  FSWatermarkSettings settings;
  FS_WATERMARK_HANDLE watermark = NULL;
  FS_BOOL return_value;

  settings.flags = e_FSWatermarkFlagsFlagASPageContents | e_FSWatermarkFlagsFlagOnTop;
  settings.offset_x = 0.f;
  settings.offset_y = 0.f;
  settings.opacity = 90;
  settings.position = e_FSPosBottomRight;
  settings.rotation = 0.0f;
  settings.scale_x = 0.1f;
  settings.scale_y = 0.1f;

  FSDK_Watermark_Create2(doc, page, settings, &watermark);
  FSDK_Watermark_InsertToPage(watermark, page, &return_value);
  FSDK_Watermark_Release(watermark);
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  wchar_t output_file[MAX_FILE_PATH] = { 0 }; 
  wchar_t output_remove_file[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;
  int page_count, i = 0;
  FS_BOOL return_result = FALSE;
  int n_count;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lswatermark/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lswatermark_add.pdf", output_directory);
  swprintf_s(output_remove_file, MAX_FILE_PATH, L"%lswatermark_remove.pdf", output_directory);

  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  FSDK_PDFDoc_GetPageCount(doc, &page_count);
  for (i = 0; i < page_count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    FS_PROGRESSIVE_HANDLE return_value = NULL;
    wchar_t wm_bmp[MAX_FILE_PATH] = { 0 };
    wchar_t wm_image[MAX_FILE_PATH] = { 0 };

    FSDK_PDFDoc_GetPage(doc, i, &page);

    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);

    AddTextWatermark(doc, page);

    swprintf_s(wm_bmp, MAX_FILE_PATH, L"%lswatermark.bmp", input_path);
    AddBitmapWatermark(doc, page, wm_bmp);

    swprintf_s(wm_image, MAX_FILE_PATH, L"%lssdk.png", input_path);
    AddImageWatermark(doc, page, wm_image);
    AddSingleWatermark(doc, page);
    FSDK_PDFPage_Release(page);
  }

  error_code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }
  printf("Add watermarks to PDF file.\n");

  error_code = FSDK_PDFDoc_GetPageCount(doc, &n_count);
  for (i = 0; i < n_count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    FS_BOOL has_watermark;
    FS_PROGRESSIVE_HANDLE return_value = NULL;

    FSDK_PDFDoc_GetPage(doc, i, &page);

    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);

    FSDK_PDFPage_HasWatermark(page, &has_watermark);
    if (has_watermark) {
      FS_BOOL remove_result;
      FSDK_PDFPage_RemoveAllWatermarks(page, &remove_result);
    }
  }

  error_code = FSDK_PDFDoc_SaveAs(doc, output_remove_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }
  printf("Remove watermarks from PDF file.\n");
  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();

  return err_ret;
}
