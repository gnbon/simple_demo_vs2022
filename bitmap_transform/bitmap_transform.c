// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to do bitmap transformation,
// such as flipping, stretching, and so on.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_image_c.h"

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

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

void SaveBitmap(FS_BITMAP_HANDLE bitmap, const wchar_t* file_path) {
  FS_IMAGE_HANDLE image = NULL;
  FS_BOOL result_value;
  FSDK_Image_Create(&image);

  FSDK_Image_AddFrame(image, bitmap, &result_value);
  FSDK_Image_SaveAs(image, file_path, &result_value);
  FSDK_Image_Release(image);
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  wchar_t input_file[MAX_FILE_PATH] = { 0 };

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsbitmap_transform/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsFoxit.bmp", input_path);
  {
    FS_IMAGE_HANDLE image = NULL;
    FS_BITMAP_HANDLE bitmap = NULL;
    int width = 0, height = 0;
    FS_BITMAP_HANDLE stretch_bitmap = NULL;
    wchar_t output_file[MAX_FILE_PATH] = { 0 };
    int left = 0, top = 0;
    FS_BITMAP_HANDLE transform_bitmap = NULL;
    FSMatrix matrix;
    FS_BITMAP_HANDLE flip_bitmap = NULL;
    FS_BITMAP_HANDLE swap_xy_bitmap = NULL;
    FSRectI rect;
    FS_BITMAP_HANDLE clone_bitmap = NULL;

    FSDK_Image_Create0(input_file, &image);
    FSDK_Image_GetFrameBitmap(image, 0, &bitmap);

    FSDK_Bitmap_GetWidth(bitmap, &width);
    FSDK_Bitmap_GetHeight(bitmap, &height);

    // Stretch bitmap.
    FSDK_Bitmap_StretchTo(bitmap,  width / 2, height * 2, e_FSDownsample, NULL, &stretch_bitmap);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsstetch_bitmap_Foxit.bmp", output_directory);
    SaveBitmap(stretch_bitmap, output_file);
    FSDK_Bitmap_Release(stretch_bitmap);

    // Transform bitmap.
    matrix.a = width * 2.0f;
    matrix.b = 0.f;
    matrix.c = 0.f;
    matrix.d = height / 2.0f;
    matrix.e = 0.f;
    matrix.f = 0.f;
    FSDK_Bitmap_TransformTo(bitmap, matrix, e_FSDownsample, left, top, NULL, &transform_bitmap);
    swprintf_s(output_file, MAX_FILE_PATH, L"%lstransform_bitmap_Foxit.bmp", output_directory);
    SaveBitmap(transform_bitmap, output_file);
    FSDK_Bitmap_Release(transform_bitmap);

    // Vertical and horizontal flip bitmap.
    FSDK_Bitmap_Flip(bitmap, TRUE, TRUE, &flip_bitmap);
    swprintf_s(output_file, MAX_FILE_PATH, L"%lsflip_horz_vert_Foxit.bmp", output_directory);
    SaveBitmap(flip_bitmap, output_file);
    FSDK_Bitmap_Release(flip_bitmap);

    // Swap the X and Y axis, and then vertical and horizontal flip bitmap.
    FSDK_Bitmap_SwapXY(bitmap, TRUE, TRUE, NULL, &swap_xy_bitmap);
    swprintf_s(output_file, MAX_FILE_PATH, L"%lsswap_xy_flip_horz_vert_Foxit.bmp", output_directory);
    SaveBitmap(swap_xy_bitmap, output_file);
    FSDK_Bitmap_Release(swap_xy_bitmap);

    // Calculate the bounding box according to the given background color.
    FSDK_Bitmap_CalculateBBoxByColor(bitmap, 0xFFFDFDFD, &rect);
    if ((rect.bottom - rect.top) > 0 && (rect.right - rect.left) > 0) {
      FSDK_Bitmap_Clone(bitmap, &rect, &clone_bitmap);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lscalculate_bbox_by_color_0xFFFDFDFD_foxit.bmp", output_directory);
      SaveBitmap(clone_bitmap, output_file);
      FSDK_Bitmap_Release(clone_bitmap);
    } else {
      printf(
        "[Warning] The result of bounding box (according to the given background color) is an empty rectangle.\r\n");
    }

    // Detect the bounding box of content according to the given color difference between content and margin.
    FSDK_Bitmap_DetectBBoxByColorDiffer(bitmap, 20, 64, &rect);
    if ((rect.bottom - rect.top) > 0 && (rect.right - rect.left) > 0) {
      FSDK_Bitmap_Clone(bitmap, &rect, &clone_bitmap);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsdetect_bbox_by color_differ_20_64_Foxit.bmp", output_directory);
      SaveBitmap(clone_bitmap, output_file);
      FSDK_Bitmap_Release(clone_bitmap);
    } else {
      printf(
        "[Warning] The result of bounding box (according to the given color difference between content and margin)"
        " is an empty rectangle.\r\n");
    }
  }
  FSDK_Library_Release();
  printf("bitmap transform done.\n");
  return err_ret;
}