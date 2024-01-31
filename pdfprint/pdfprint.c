// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to print PDF pages.

#include <time.h>
#include<direct.h>
#include <stdio.h>
#include <windows.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfgraphicsobject_c.h"
#include "../../../include/fs_pdfpage_c.h"

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

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;
  HDC hdc_print = NULL;  // Printer DC handle
  wchar_t dev_string[120];  // An array for WIN.INI data
  wchar_t *printer = NULL, *driver = NULL;  // Printer name and driver name
  wchar_t *port = NULL;  // Port name
  wchar_t* context = NULL;
  wchar_t print_doc_name[128];
  DOCINFOW doc_info;
  int error, page_count, i;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsannotations.pdf", input_path);

  // Load PDF document.
  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    wprintf(L"[Failed] Cannot load PDF document %ls.\r\nError Code: %d\r\n", input_file, error_code);
    return 1;
  }


  // Retrieve the printer, printer driver, and output-port names from WIN.INI.
  GetProfileStringW(L"windows", L"device", L",,,", dev_string, 120);

  // Parse the string of names, setting ptrs as required.
  // If the string contains the required names, use them to create a device context.
  if ((printer = wcstok_s(dev_string, (const wchar_t*)L",", &context)) && (driver = wcstok_s((wchar_t *)NULL, (const wchar_t *)L", ", &context)) &&
    (port = wcstok_s((wchar_t *)NULL, (const wchar_t *)L", ", &context))) {
    hdc_print = CreateDCW(driver, printer, port, NULL);
  }

  if (!hdc_print) {
    printf("[Failed] Cannot create HDC for print.\r\n");
    return 1;
  }

  swprintf_s(print_doc_name, 128, L"FoxitPDFSDK_PrintingTest_%ls", input_file);

  ZeroMemory(&doc_info, sizeof(DOCINFOW));
  doc_info.cbSize = sizeof(DOCINFOW);
  doc_info.lpszDocName = print_doc_name;
  doc_info.lpszOutput = NULL;
  doc_info.fwType = 0;
  // Begin a print job by calling the StartDoc function.
  error = StartDocW(hdc_print, &doc_info);
  if (error == SP_ERROR) {
    MessageBoxA(NULL, "Error", "Error, Printing", MB_OK | MB_ICONEXCLAMATION);
    return 1;
  }

  FSDK_PDFDoc_GetPageCount(doc, &page_count);
  for (i = 0; i < page_count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    float f_width, f_height;
    int width, height;
	float device_width, device_height, x_scale, y_scale, scale;
    float x_offset = 0.0;
    float y_offset = 0.0;
	RECT rect;
    FSMatrix matrix;
    FSRotation return_rotation;
    HBRUSH hbrush;
    FS_RENDERER_HANDLE render_print = NULL;
    FSRectI clip_rect;
    FS_PROGRESSIVE_HANDLE progressive2;

    FSDK_PDFDoc_GetPage(doc, 0, &page);
    // Parse page.
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);

	FSDK_PDFPage_GetWidth(page, &f_width);
    width = (int)(f_width);
    FSDK_PDFPage_GetHeight(page, &f_height);
    height = (int)f_height;

    // Inform the driver that the application is about to begin sending data.
    error = StartPage(hdc_print);
    if (error == SP_ERROR) {
      MessageBoxA(NULL, "Error", "Error, Printing", MB_OK | MB_ICONEXCLAMATION);
      return 1;
    }

    device_width = (float)GetDeviceCaps(hdc_print, HORZRES);
    device_height = (float)GetDeviceCaps(hdc_print, VERTRES);
    x_scale = device_width / width;
    y_scale = device_height / height;

    scale = x_scale < y_scale ? x_scale : y_scale;

    if (x_scale < y_scale) {
      y_offset = (device_height - scale * height) / 2;
      device_height = scale * height;
    }
    else {
      x_offset = (device_width - scale * width) / 2;
      device_width = scale * width;
    }

    rect.left = (LONG)x_offset;
    rect.top = (LONG)y_offset;
	rect.right = (LONG)(device_width + x_offset);
	rect.bottom = (LONG)(device_height + y_offset);

    FSDK_PDFPage_GetRotation(page, &return_rotation);
    FSDK_PDFPage_GetDisplayMatrix(page, rect.left, rect.top, (int)device_width, (int)device_height, e_FSRotation0, &matrix);

    hbrush = CreateSolidBrush(RGB(0xff, 0xff, 0xff));
    FillRect(hdc_print, &rect, hbrush);
    DeleteObject(hbrush);


    FSDK_Renderer_Create1(hdc_print, dev_string, &render_print);

    clip_rect.left = rect.left;
    clip_rect.top = rect.top;
    clip_rect.right = rect.right;
    clip_rect.bottom = rect.bottom;
    FSDK_Renderer_SetClipRect(render_print, &clip_rect);
    FSDK_Renderer_EnableForPrint(render_print, TRUE);

    FSDK_Renderer_StartRender(render_print, page, matrix, NULL, &progressive2);

    // Inform the spooler that the page is complete.
    EndPage(hdc_print);
    FSDK_PDFPage_Release(page);
  }
  // Inform the driver that document has ended.
  EndDoc(hdc_print);
  // Release the device context.
  DeleteDC(hdc_print);

  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();
  printf("Print a PDF file.\n");

  return err_ret;
}

