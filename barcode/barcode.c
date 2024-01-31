// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to generate barcode.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_barcode_c.h"
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

void Save2Image(FS_BITMAP_HANDLE bitmap, const wchar_t* bmp_name) {
  // Add the bitmap to image and save the image.
  FS_IMAGE_HANDLE image = NULL;
  FS_BOOL return_result;
  wchar_t output_filepath[MAX_FILE_PATH] = { 0 };

  FSDK_Image_Create(&image);
  FSDK_Image_AddFrame(image, bitmap, &return_result);

  swprintf_s(output_filepath, MAX_FILE_PATH, L"%lsbarcode%ls", output_path, bmp_name);
  FSDK_Image_SaveAs(image, output_filepath, &return_result);
  FSDK_Image_Release(image);
}

// Generate barcode in different type and save each of them as an image file.
void Barcode_GenerateEachType(const wchar_t* code_string, FSFormat code_format, int unit_width, int unit_height, FSQRErrorCorrectionLevel qr_level, const wchar_t* bmp_name) {
  FS_BARCODE_HANDLE barcode = NULL;
  FS_BITMAP_HANDLE bitmap = NULL;

  FSDK_Barcode_Create(&barcode);
  FSDK_Barcode_GenerateBitmap(barcode, code_string, code_format, unit_width, unit_height, qr_level, &bitmap);

  Save2Image(bitmap, bmp_name);
  FSDK_Barcode_Release(barcode);
}

// Generate all types of barcode and save each of them as an image file.
void Barcode_GenerateAll() {
  // Strings used as barcode content.
  const wchar_t* sz_code_string[8] = { L"TEST-SHEET", L"102030405060708090", L"80674313", L"890444000335", L"9780804816632", L"070429", L"Unknown - change me!", L"TestForBarcodeQrCode" };
  // Barcode format types
  FSFormat code_format[8] = { e_FSFormatCode39, e_FSFormatCode128,
    e_FSFormatEAN8, e_FSFormatUPCA,
    e_FSFormatEAN13, e_FSFormatITF,
    e_FSFormatPDF417, e_FSFormatQRCode };

  // Image names for the saved image files.(except QR code)
  const wchar_t* sz_bmp_name[7] = { L"/code39_TEST-SHEET.bmp", L"/CODE_128_102030405060708090.bmp",
    L"/EAN_8_80674313.bmp", L"/UPC_A_890444000335.bmp", L"/EAN_13_9780804816632.bmp",
    L"/ITF_070429.bmp", L"/PDF_417_Unknown.bmp" };

  //Format error correction level of QR code.
  FSQRErrorCorrectionLevel sz_qr_level[4] = { e_FSQRCorrectionLevelLow, e_FSQRCorrectionLevelMedium,
    e_FSQRCorrectionLevelQuater, e_FSQRCorrectionLevelHigh };
  //Image names for the saved image files for QR code.
  const wchar_t* bmp_qr_name[4] = { L"/QR_CODE_TestForBarcodeQrCode_L.bmp", L"/QR_CODE_TestForBarcodeQrCode_M.bmp",
    L"/QR_CODE_TestForBarcodeQrCode_Q.bmp", L"/QR_CODE_TestForBarcodeQrCode_H.bmp" };

  // Unit width for barcode in pixels, preferred value is 1-5 pixels.
  int unit_width = 2;
  // Unit height for barcode in pixels, preferred value is >= 20 pixels.
  int unit_height = 120;
  int i = 0;
  int j = 0;

  // Generate barcode for different types.
  for (i = 0; i < 8; i++) {
    if (code_format[i] != e_FSFormatQRCode) {
      // Not QR code
      Barcode_GenerateEachType(sz_code_string[i], code_format[i], unit_width, unit_height, (FSQRErrorCorrectionLevel)0, sz_bmp_name[i]);
    } else {
      // QR code
      //Generate for each format error correction level.
      for (j = 0; j < 4; j++)
        Barcode_GenerateEachType(sz_code_string[i], code_format[i], unit_width, unit_height, sz_qr_level[j], bmp_qr_name[j]);
    }
  }
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };

  FSErrorCode error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsbarcode/", output_path);
  _wmkdir(output_directory);
  // Generate all types of barcode
  Barcode_GenerateAll();
  printf("Barcode demo.\n");
  FSDK_Library_Release();
  return err_ret;
}