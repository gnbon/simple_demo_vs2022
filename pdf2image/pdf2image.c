// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file is a demo demonstrate how to convert a PDF file to one or multiple image files.

// Include Foxit SDK header files.
#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <windows.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_image_c.h"
#include "../../../include/fs_render_c.h"

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

wchar_t output_directory[MAX_FILE_PATH] = { 0 };

void Save2Image(FS_BITMAP_HANDLE bitmap, int page_index, const wchar_t* ext, FS_BOOL from_memory) {
  // Add the bitmap to image and save the image.
  FS_IMAGE_HANDLE image = NULL;
  wchar_t s_new[100];
  FS_BOOL return_addframe;
  wchar_t s[MAX_FILE_PATH];
  FS_BOOL return_saveas;

  FSDK_Image_Create(&image);
  FSDK_Image_AddFrame(image, bitmap, &return_addframe);

  if (from_memory)
    swprintf_s(s_new, 100, L"AboutFoxit_from_memory_%d", page_index);
  else
    swprintf_s(s_new, 100, L"AboutFoxit_%d", page_index);

  swprintf_s(s, MAX_FILE_PATH, L"%ls%ls%ls", output_directory, s_new, ext);

  FSDK_Image_SaveAs(image, s, &return_saveas);

  FSDK_Image_Release(image);
  wprintf(L"Save page %d into a picture of %ls format %ls.\n", page_index, ext, (from_memory ? L" from memory" : L""));
}

void PDF2Image(FS_PDFDOC_HANDLE doc, FS_BOOL from_memory) {
  const wchar_t* support_image_extends[] = { L".bmp", L".jpg", L".jpeg", L".png", L".jpx", L".jp2" };
  // Get page count
  int page_count;
  int i = 0;
  FSDK_PDFDoc_GetPageCount(doc, &page_count);
  for (i = 0; i < page_count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    FSErrorCode code = e_FSErrSuccess;
    float fwidth;
    int width;
    float fheight;
    int height;
    FSRotation return_rotation;
    FSMatrix return_matrix;
    FS_BITMAP_HANDLE bitmap = NULL;
    FS_RENDERER_HANDLE render = NULL;
    unsigned int j = 0;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FS_PROGRESSIVE_HANDLE return_startrender;

    code = FSDK_PDFDoc_GetPage(doc, i, &page);
    if (code != e_FSErrSuccess) {
      FSDK_PDFPage_Release(page);
      return;
    }
    // Parse page.
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);

    FSDK_PDFPage_GetWidth(page, &fwidth);
    width = (int)(fwidth);

    FSDK_PDFPage_GetHeight(page, &fheight);
    height = (int)(fheight);

    FSDK_PDFPage_GetRotation(page, &return_rotation);

    FSDK_PDFPage_GetDisplayMatrix(page, 0, 0, width, height, return_rotation, &return_matrix);
    // Prepare a bitmap for rendering.
    FSDK_Bitmap_Create(width, height, e_FSDIBArgb, NULL, 0, &bitmap);
    FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, NULL);

    // Render page
    FSDK_Renderer_Create(bitmap, FALSE, &render);
    FSDK_Renderer_StartRender(render, page, return_matrix, NULL, &return_startrender);
    for (j = 0; j < sizeof(support_image_extends) / sizeof(support_image_extends[0]); j++) {
      const wchar_t* extend = support_image_extends[j];
      Save2Image(bitmap, i, extend, from_memory);
    }

    FSDK_Bitmap_Release(bitmap);
    FSDK_Renderer_Release(render);
    FSDK_PDFPage_Release(page);
  }
}

void SaveTiffImage(FS_PDFDOC_HANDLE doc, FS_BOOL from_memory) {
  const wchar_t* support_multi_image[] = { L".tif", L".tiff" };
  int i = 0;
  for (i = 0; i < sizeof(support_multi_image) / sizeof(support_multi_image[0]); i++) {
    wchar_t s[MAX_FILE_PATH];
    FS_IMAGE_HANDLE image = NULL;
    int page_count;
    int page_index = 0;
    FS_BOOL return_saveas;

    swprintf_s(s, MAX_FILE_PATH, L"%lsAboutFoxit", output_directory);
    if (from_memory) swprintf_s(s, MAX_FILE_PATH, L"%ls_from_memory", s);
    swprintf_s(s, MAX_FILE_PATH, L"%ls%ls", s, support_multi_image[i]);

    FSDK_Image_Create(&image);

    FSDK_PDFDoc_GetPageCount(doc, &page_count);
    for (page_index = 0; page_index < page_count; page_index++) {
      FS_PDFPAGE_HANDLE page = NULL;
      FS_PROGRESSIVE_HANDLE return_value = NULL;
      float fwidth;
      int width = 0;
      float fheight;
      int height = 0;
      FSRotation return_rotation;
      FSMatrix matrix;
      FS_BITMAP_HANDLE bitmap;
      FS_RENDERER_HANDLE render = NULL;
      FS_BOOL return_addframe;
      FS_PROGRESSIVE_HANDLE return_startrender = NULL;

      FSDK_PDFDoc_GetPage(doc, page_index, &page);
      // Parse page.
      FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);

      FSDK_PDFPage_GetWidth(page, &fwidth);
      width = (int)(fwidth);

      FSDK_PDFPage_GetHeight(page, &fheight);
      height = (int)(fheight);

      FSDK_PDFPage_GetRotation(page, &return_rotation);

      FSDK_PDFPage_GetDisplayMatrix(page, 0, 0, width, height, return_rotation, &matrix);
      // Prepare a bitmap for rendering.

      FSDK_Bitmap_Create(width, height, e_FSDIBArgb, NULL, 0, &bitmap);
      FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, NULL);

      // Render page
      FSDK_Renderer_Create(bitmap, FALSE, &render);

      FSDK_Renderer_StartRender(render, page, matrix, NULL, &return_startrender);
      FSDK_Renderer_Release(render);
      FSDK_Image_AddFrame(image, bitmap, &return_addframe);
      FSDK_Bitmap_Release(bitmap);
      FSDK_PDFPage_Release(page);
    }

    FSDK_Image_SaveAs(image, s, &return_saveas);
    FSDK_Image_Release(image);
    wprintf(L"Save pdf file into a picture of %ls format %ls\n", support_multi_image[i], (from_memory ? L" from memory" : L""));
  }
}

#if defined(_WIN32) || defined(_WIN64)
void SaveHDC2BMPFile(HDC hdc_display, int page_index) {
  // Get HBITMAP from HDC.
  HBITMAP h_bitmap = NULL;
  BITMAP bitmap;
  unsigned int bitmap_byte_per_line = 0;
  auto bitmap_byte_size = 0;
  unsigned char* hbitmap_data = NULL;
  wchar_t bmp_path[200];
  FILE* save_file = NULL;
  int file_size = 0;
  unsigned char file_head_buf[14];
  unsigned char bmp_head_buf[40];
  int bitmapwidth, bitmapheight = 0;

  h_bitmap = (HBITMAP)GetCurrentObject(hdc_display, OBJ_BITMAP);
  if (!h_bitmap) {
    printf("[Failed] Cannot get OBJ_BITMAP of the HDC for rendering page (page index = %d).\r\n", page_index);
    return;
  }

  // Get BITMAP for HBTIMAP.
  GetObject(h_bitmap, sizeof(BITMAP), &bitmap);
  bitmap_byte_per_line = (bitmap.bmWidth * bitmap.bmBitsPixel + 7) / 8;
  bitmap_byte_per_line = (bitmap_byte_per_line + 3) / 4 * 4;
  bitmap_byte_size = bitmap_byte_per_line * bitmap.bmHeight;

  // Get data of HBITMAP
  hbitmap_data = (unsigned char*)malloc(sizeof(unsigned char) * bitmap_byte_size);
  GetBitmapBits(h_bitmap, bitmap_byte_size, hbitmap_data);
  if (!hbitmap_data) {
    printf("[Failed] Cannot get data of HBITMAP for the HDC for rendering page (page index = %d).\r\n", page_index);
    return;
  }

  swprintf_s(bmp_path, 200, L"%lsrender_by_hdc_page%d.bmp", output_directory, page_index);

  _wfopen_s(&save_file, bmp_path, L"wb");

  if (!save_file) {
    wprintf(L"[Failed] Cannot open the save file %ls.\r\n", bmp_path);
    return;
  }

  // Save to a .bmp file.
  file_size = 14 + 40 + bitmap_byte_size;
  // Prepare file header buffer
  memset(file_head_buf, 0, 14);
  file_head_buf[0] = 'B';
  file_head_buf[1] = 'M';
  file_head_buf[2] = (unsigned char)file_size;
  file_head_buf[3] = (unsigned char)(file_size >> 8);
  file_head_buf[4] = (unsigned char)(file_size >> 16);
  file_head_buf[5] = (unsigned char)(file_size >> 24);
  file_head_buf[6] = file_head_buf[7] = file_head_buf[8] = file_head_buf[9] = 0;
  file_head_buf[10] = 54;
  file_head_buf[11] = file_head_buf[12] = file_head_buf[13] = 0;

  // Write file header buffer to file.
  fwrite(file_head_buf, 14, 1, save_file);

  // Prepare bmp header buffer
  memset(bmp_head_buf, 0, 40);
  bitmapwidth = bitmap.bmWidth;
  bitmapheight = bitmap.bmHeight;
  memset(bmp_head_buf, 0, 40);
  bmp_head_buf[0] = 40;
  bmp_head_buf[4] = (unsigned char)bitmapwidth;
  bmp_head_buf[5] = (unsigned char)(bitmapwidth >> 8);
  bmp_head_buf[6] = (unsigned char)(bitmapwidth >> 16);
  bmp_head_buf[7] = (unsigned char)(bitmapwidth >> 24);
  bmp_head_buf[8] = (unsigned char)(-1 * bitmapheight);
  bmp_head_buf[9] = (unsigned char)((-1 * bitmapheight) >> 8);
  bmp_head_buf[10] = (unsigned char)((-1 * bitmapheight) >> 16);
  bmp_head_buf[11] = (unsigned char)((-1 * bitmapheight) >> 24);
  bmp_head_buf[12] = 1; //plane always 1
  bmp_head_buf[14] = (char)bitmap.bmBitsPixel;
  // Write bmp header buffer to file.
  fwrite(bmp_head_buf, 40, 1, save_file);

  // Write HBITMAP data to file.
  fwrite(hbitmap_data, 1, bitmap_byte_size, save_file);

  // Close file
  fclose(save_file);
  // Release resources.
  free(hbitmap_data);
}

void PDF2ImageByDC(FS_PDFDOC_HANDLE doc) {
  // (Only for Windows) Show how to use HDC to render PDF page and then save the rendering result of HDC to .bmp file.
  int page_count, i = 0;
  FSDK_PDFDoc_GetPageCount(doc, &page_count);
  for (i = 0; i < page_count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    FS_PROGRESSIVE_HANDLE return_value = NULL;
    float fwidth, fheight, x_scale, y_scale, scale;
    int page_width = 0, page_height = 0, device_width = 0,  device_height = 0;
    HDC hdc_display = NULL;
    int actual_render_width = 0;
    int actual_render_height = 0;
    int x_offset = 0;
    int y_offset = 0;
    FSRotation return_rotation;
    FSMatrix return_matrix;
    HBRUSH hbrush = NULL;
    FS_RENDERER_HANDLE render = NULL;
    FS_PROGRESSIVE_HANDLE return_startrender;

    FSDK_PDFDoc_GetPage(doc, i,&page);
    // Parse page.
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);

    FSDK_PDFPage_GetWidth(page, &fwidth);
    page_width = (int)(fwidth);
    FSDK_PDFPage_GetHeight(page, &fheight);
    page_height = (int)(fheight);

    // Create HDC for rendering PDF page.
    hdc_display = CreateDCA("DISPLAY", NULL, NULL, NULL);
    if (!hdc_display) {
      printf("[Failed] Cannot create HDC for rendering a PDF page.\r\n");
      return;
    }

    // Calculate the size of render area.
    device_width = GetDeviceCaps(hdc_display, HORZRES);
    device_height = GetDeviceCaps(hdc_display, VERTRES);
    x_scale = ((float)device_width) / page_width;
    y_scale = ((float)device_height) / page_height;
    scale = x_scale < y_scale ? x_scale : y_scale;

    if (x_scale < y_scale) {
      actual_render_width = device_width;
      actual_render_height = (int)(scale * page_height + 0.5);
    }
    else {
      actual_render_width = (int)(scale * page_width + 0.5);
      actual_render_height = device_height;
    }

    // Prepare display_matrix for displaying the page.
    FSDK_PDFPage_GetRotation(page, &return_rotation);
    FSDK_PDFPage_GetDisplayMatrix(page, x_offset, y_offset, actual_render_width, actual_render_height, return_rotation, &return_matrix);

    // Fill the HDC with white brush.
    hbrush = CreateSolidBrush(RGB(0xff, 0xff, 0xff));
    if (hbrush) {
      RECT rect = { (LONG)x_offset, (LONG)y_offset, (LONG)(actual_render_width + x_offset), (LONG)(actual_render_height + y_offset) };
      FillRect(hdc_display, &rect, hbrush);
      DeleteObject(hbrush);
    }

    // Render PDF page by HDC.
    FSDK_Renderer_Create0(hdc_display, &render);
    FSDK_Renderer_StartRender(render, page, return_matrix, NULL, &return_startrender);
    SaveHDC2BMPFile(hdc_display, i);
    // Release resources.
    DeleteDC(hdc_display);
    FSDK_Renderer_Release(render);
    FSDK_PDFPage_Release(page);
  }
}

#endif  // #if defined(_WIN32) || defined(_WIN64)

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH];

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdf2image/", output_path);
  _wmkdir(output_directory);
  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);

  {
    FS_PDFDOC_HANDLE doc = NULL;
    FSErrorCode code = e_FSErrSuccess;
    DEVMODEA new_screen_settings;

    code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (code != e_FSErrSuccess) return 1;
    code = FSDK_PDFDoc_Load(doc, NULL);
    if (code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
      return 1;
    }
    PDF2Image(doc, FALSE);
    SaveTiffImage(doc, FALSE);

    // Following code is about how to use HDC (in Windows) to render PDF page by Foxit PDF SDK,
    // and try to save the result in HDC to .bmp file by Windows API.
    // This sampl will not be triggered when the demo runs in a remote desktop,
    // because "save the result in HDC to .bmp file by Windows API" may not do as expected.
    // User can improve "save the result in HDC to .bmp file by Windows API" if necessary.

    memset(&new_screen_settings, 0, sizeof(new_screen_settings));
    new_screen_settings.dmSize = sizeof(new_screen_settings);
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &new_screen_settings);
    if (0 != strcmp((char*)new_screen_settings.dmDeviceName, "RDPUDD"))
      PDF2ImageByDC(doc);

    FSDK_PDFDoc_Release(doc);
  }

  {
    FILE* file = NULL;
    errno_t error;
    size_t file_size = 0;
    char* buffer = NULL;
    FS_PDFDOC_HANDLE doc_memory;
    FSErrorCode code = e_FSErrSuccess;

    printf("Load document from memory.\n");

    error = _wfopen_s(&file, input_file, L"rb+");
    fseek(file, 0, SEEK_END);
    file_size = (size_t)ftell(file);
    buffer = (char*)malloc(file_size * sizeof(char));
    memset(buffer, 0, file_size);

    fseek(file, 0, SEEK_SET);
    fread(buffer, sizeof(char), file_size, file);
    fclose(file);

    code = FSDK_PDFDoc_Create0(input_file, &doc_memory);
    if (code != e_FSErrSuccess) return 1;
    code = FSDK_PDFDoc_Load(doc_memory, NULL);
    if (code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc_memory);
      wprintf(L"The Doc [%ls] Error from memory: %d\n", input_file, code);
      return 1;
    }
    PDF2Image(doc_memory, TRUE);
    SaveTiffImage(doc_memory, TRUE);
    free(buffer);
    FSDK_PDFDoc_Release(doc_memory);
  }

  FSDK_Library_Release();
  return err_ret;
}