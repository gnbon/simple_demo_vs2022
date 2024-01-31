// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to reflow PDF pages.

#include <time.h>
#include<direct.h>
#include <stdio.h>
#include <math.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_reflowpage_c.h"
#include "../../../include/fs_render_c.h"
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

static const wchar_t* output_directory = L"../output_files/pdfreflow/";

const void SaveBitmap(FS_BITMAP_HANDLE bitmap, int index, const wchar_t* file_name) {
  FSRectF margin;
  FSPointF size;
  FSRectI rect;
  FS_IMAGE_HANDLE image = NULL;
  FS_BOOL return_value;
  wchar_t save_path[MAX_FILE_PATH] = { 0 };

  margin.left = 50;
  margin.bottom = 30;
  margin.right = 30;
  margin.top = 30;

  size.x = 480;
  size.y = 800;

  rect.left = 0;
  rect.bottom = 0;
  rect.right = (int)size.x;
  rect.top = (int)margin.top;
  FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, &rect);
  rect.left = 0;
  rect.bottom = (int)(size.y - margin.bottom);
  rect.right = (int)size.x;
  rect.top = (int)size.y;
  FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, &rect);

  FSDK_Image_Create(&image);

  FSDK_Image_AddFrame(image, bitmap, &return_value);

  swprintf_s(save_path, MAX_FILE_PATH, L"%lsreflow%ls%d.bmp", output_directory, file_name, index);

  FSDK_Image_SaveAs(image, save_path, &return_value);
  FSDK_Image_Release(image);
}

// The change for the size of the picture depends on the size of the content of reflow page.
void ReflowSingle(FS_PDFDOC_HANDLE doc) {
  FSRectF margin;
  FSPointF size;
  int page_count, i = 0;

  margin.left = 50;
  margin.bottom = 30;
  margin.right = 30;
  margin.top = 30;

  size.x = 480;
  size.y = 800;

  FSDK_PDFDoc_GetPageCount(doc, &page_count);
  for (i = 0; i < page_count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    float content_height;
    float content_width;
    FS_BITMAP_HANDLE bitmap = NULL;
    FS_RENDERER_HANDLE renderer = NULL;
    FSMatrix matrix;
    const wchar_t* file_name = L"_single_";

    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FS_PROGRESSIVE_HANDLE progressive1 = NULL;
    FS_PROGRESSIVE_HANDLE progressive2 = NULL;
    FS_REFLOWPAGE_HANDLE reflow_page = NULL;

    FSDK_PDFDoc_GetPage(doc, i, &page);
    // Parse PDF page.
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);
    FSDK_ReflowPage_Create(page, &reflow_page);
    // Set some arguments used for parsing the relfow page.
    FSDK_ReflowPage_SetLineSpace(reflow_page, 0);
    FSDK_ReflowPage_SetScreenMargin(reflow_page, (int)margin.left, (int)margin.top,
      (int)margin.right, (int)margin.bottom);
    FSDK_ReflowPage_SetScreenSize(reflow_page, size.x, size.y);
    FSDK_ReflowPage_SetZoom(reflow_page, 100);
    FSDK_ReflowPage_SetParseFlags(reflow_page, e_FSReflowPageFlagsNormal);
    // Parse reflow page.

    FSDK_ReflowPage_StartParse(reflow_page, NULL, &progressive1);

    // Get actual size of content of reflow page. The content size does not contain the margin.
    FSDK_ReflowPage_GetContentHeight(reflow_page, &content_height);

    FSDK_ReflowPage_GetContentWidth(reflow_page, &content_width);

    // Create a bitmap for rendering the reflow page. The bitmap size contains the margin.
    FSDK_Bitmap_Create((int)(content_width + margin.left + margin.right), (int)(content_height + margin.top + margin.bottom), e_FSDIBArgb, NULL, 0, &bitmap);
    FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, NULL);
    // Render reflow page.
    FSDK_Renderer_Create(bitmap, FALSE, &renderer);
    FSDK_ReflowPage_GetDisplayMatrix(reflow_page, 0, 0, (int)content_width, (int)content_height, e_FSRotation0, &matrix);

    FSDK_Renderer_StartRenderReflowPage(renderer, reflow_page, matrix, NULL, &progressive2);
    SaveBitmap(bitmap, i, file_name);

    FSDK_Renderer_Release(renderer);
    FSDK_Bitmap_Release(bitmap);
    FSDK_ReflowPage_Release(reflow_page);
    FSDK_PDFPage_Release(page);
  }
}

// Fixed bitmap size, just to simulate split screen situation.
void ReflowContinuous(FS_PDFDOC_HANDLE doc) {
  FSRectF margin;
  float offset_y = 0;
  int page_count;
  int bitmap_index = 0, i = 0;
  const wchar_t* file_name = L"_continuous_";
  float display_height = 0; 
  FSPointF size;
  FS_BITMAP_HANDLE bitmap = NULL;

  margin.left = 50;
  margin.bottom = 30;
  margin.right = 30;
  margin.top = 30;

  size.x = 480;
  size.y = 800;
  display_height = size.y - margin.top - margin.bottom;

  FSDK_Bitmap_Create((int)size.x, (int)size.y, e_FSDIBArgb, NULL, 0, &bitmap);

  FSDK_PDFDoc_GetPageCount(doc, &page_count);

  for (i = 0; i < page_count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FS_REFLOWPAGE_HANDLE reflow_page = NULL;
    FS_PROGRESSIVE_HANDLE progressive3 = NULL;
    FS_RENDERER_HANDLE renderer = NULL;
    FS_PROGRESSIVE_HANDLE progressive4 = NULL;

    float content_height;
    float content_width;
    FSMatrix matrix;
    int rate_need_screen_count = 0;

    FSDK_PDFDoc_GetPage(doc, i, &page);

    // Parse PDF page.
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);
    FSDK_ReflowPage_Create(page, &reflow_page);
    // Set some arguments used for parsing the relfow page.
    FSDK_ReflowPage_SetLineSpace(reflow_page, 0);
    FSDK_ReflowPage_SetScreenMargin(reflow_page, (int)margin.left, (int)margin.top,
      (int)margin.right, (int)margin.bottom);
    FSDK_ReflowPage_SetScreenSize(reflow_page, size.x, size.y);
    FSDK_ReflowPage_SetZoom(reflow_page, 100);
    FSDK_ReflowPage_SetParseFlags(reflow_page, e_FSWithImage);

    FSDK_ReflowPage_SetTopSpace(reflow_page, offset_y);

    // Parse reflow page.
    FSDK_ReflowPage_StartParse(reflow_page, NULL, &progressive3);

    // Get actual size of content of reflow page.
    // The content size does not contain the margin but contains the top space.
    FSDK_ReflowPage_GetContentHeight(reflow_page, &content_height);
    FSDK_ReflowPage_GetContentWidth(reflow_page, &content_width);
    // Render reflow page.
    FSDK_Renderer_Create(bitmap, FALSE, &renderer);

    FSDK_ReflowPage_GetDisplayMatrix(reflow_page, 0, 0, (int)content_width, (int)content_height, e_FSRotation0, &matrix);

    FSDK_Renderer_StartRenderReflowPage(renderer, reflow_page, matrix, NULL, &progressive4);

    rate_need_screen_count = (int)(ceil(max(content_height - display_height, 0.0f) / display_height));
    if (rate_need_screen_count > 0) {
      // Before do next rendering, save current bitmap first.
      const wchar_t* file_name = L"_continuous_";
      float has_display_height = display_height;
      int j = 0;

      SaveBitmap(bitmap, bitmap_index++, file_name);
      for (j = 0; j < rate_need_screen_count; j++) {
        FSMatrix matrix;
        FS_RENDERER_HANDLE renderer = NULL;
        FS_PROGRESSIVE_HANDLE progressive = NULL;

        // Clear the bitmap and used it to do next rendering.
        FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, NULL);
        // Render reflow page.
        FSDK_Renderer_Create(bitmap, FALSE, &renderer);
        FSDK_ReflowPage_GetDisplayMatrix(reflow_page, 0, -has_display_height, (int)content_width, (int)content_height, e_FSRotation0, &matrix);

        FSDK_Renderer_StartRenderReflowPage(renderer, reflow_page, matrix, NULL, &progressive);
        if (j != rate_need_screen_count - 1) {
          has_display_height += display_height;
          SaveBitmap(bitmap, bitmap_index++, file_name);
        } else {
          offset_y = content_height - rate_need_screen_count * display_height;
        }
      }
    } else {
      offset_y = content_height;
    }

    FSDK_ReflowPage_Release(reflow_page);
    FSDK_Renderer_Release(renderer);
    FSDK_PDFPage_Release(page);
  }
  SaveBitmap(bitmap, bitmap_index++, file_name);
  FSDK_Bitmap_Release(bitmap);
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  FS_PDFDOC_HANDLE doc = NULL;
  wchar_t input_file[MAX_FILE_PATH] = { 0 };

  FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);

  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }
  ReflowContinuous(doc);
  ReflowSingle(doc);

  FSDK_PDFDoc_Release(doc);
  printf("Reflow test.\n");

  return err_ret;
}

