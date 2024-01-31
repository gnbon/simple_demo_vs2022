// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to convert images to PDF files.

// Include Foxit SDK header files.
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include<direct.h>
#include <stdio.h>
#include <windows.h>

// #define DEBUG

#define MAX_FILE_PATH 260
#define TRUE 1
#define FALSE 0

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_image_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_convert_c.h"

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

extern "C" __declspec(dllexport) void Image2PDF(const wchar_t* input_file, const wchar_t* output_file);

void Image2PDF(const wchar_t* input_file, const wchar_t* output_file) {
  FS_IMAGE_HANDLE image = NULL;
  FS_BOOL result;
  FSImageType type;
  int count = 1, i = 0;
  FS_PDFDOC_HANDLE doc = NULL;

  FSDK_Image_Create0(input_file, &image);
  FSDK_Image_GetType(image, &type);
  if (type != e_FSJBIG2) {
    FSDK_Image_GetFrameCount(image, &count);
  } 

  FSDK_PDFDoc_Create(&doc);
  for (i = 0; i < count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    int width, height;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FSPointF pointf;
    FS_BOOL return_result;
    FSDK_Image_GetWidth(image, &width);
    FSDK_Image_GetHeight(image, &height);
    FSDK_PDFDoc_InsertPage(doc, i, (float)width, (float)height, &page);

    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);
    // Add image to page
    pointf.x = 0;
    pointf.y = 0;

    FSDK_PDFPage_AddImage(page, image, i, pointf, (float)width, (float)height, TRUE, &return_result);
    FSDK_PDFPage_Release(page);
  }
  FSDK_Image_Release(image);
  FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &result);
  FSDK_PDFDoc_Release(doc);
}

int wmain(int argc, wchar_t *argv[]) {
    int err_ret = 0;
    FSErrorCode error_code = FSDK_Library_Initialize(sn, key);
    if (error_code != e_FSErrSuccess) {
        printf("Library Initialize Error: %d\n", error_code);
        return 1;
    }

    if (argc < 2) {
        printf("Argc not enough: %d\n", argc);
        return 1;
    }

  // Convert input file to PDF document.
  
#ifdef DEBUG
  Image2PDF(argv[1], argv[2]);
#else
  Image2PDF(argv[1], NULL);
#endif
  printf("Convert %ls file to PDF file.\n", argv[1]);

  FSDK_Library_Release();
  return err_ret;
}
