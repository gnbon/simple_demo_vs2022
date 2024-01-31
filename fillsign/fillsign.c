// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to add fillsign to PDF document.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_fillsign_c.h"
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

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsfillsign/", output_path);
  _wmkdir(output_directory);


  swprintf_s(input_file, MAX_FILE_PATH, L"%lsblank.pdf", input_path);
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    FS_FILLSIGN_HANDLE fillsign = NULL;
    FSPointF point;

    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FS_FILLSIGNOBJECT_HANDLE fillsignobject_crossmark = NULL;
    FS_FILLSIGNOBJECT_HANDLE fillsignobject_checkmark = NULL;
    FS_FILLSIGNOBJECT_HANDLE fillsignobject_roundrectangle = NULL;
    FS_FILLSIGNOBJECT_HANDLE fillsignobject_typeline = NULL;
    FS_FILLSIGNOBJECT_HANDLE fillsignobject_dot = NULL;
    FS_FILLSIGNOBJECT_HANDLE text = NULL;
    FS_FILLSIGNOBJECT_HANDLE fillsignobject_signature = NULL;
    FS_BITMAP_HANDLE bitmap = NULL;
    FS_SIGNATUREFILLSIGNOBJECT_HANDLE signature = NULL;

    FS_WSTR tempstr;
    FSTextFillSignObjectData textdata;
    FSTextState textState;
    FS_FONT_HANDLE font = NULL;
    FS_BOOL isfinish;
    wchar_t pdf_new[MAX_FILE_PATH] = { 0 };

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);

    FSDK_FillSign_Create(page, &fillsign);

    point.x = 0;
    point.y = 0;
    error_code = FSDK_FillSign_AddObject(fillsign, e_FSFillSignObjectTypeCrossMark, point, 100, 100, e_FSRotation0, &fillsignobject_crossmark);
    if (error_code != e_FSErrSuccess) {
      printf("FSFillSignObjectTypeCrossMark error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_FillSign_Release(fillsign);
      return 1;
    }
    FSDK_FillSignObject_Release(fillsignobject_crossmark);

    point.x = 100;
    point.y = 100;
    error_code = FSDK_FillSign_AddObject(fillsign, e_FSFillSignObjectTypeCheckMark, point, 50, 100, e_FSRotation0, &fillsignobject_checkmark);
    if (error_code != e_FSErrSuccess) {
      printf("FSFillSignObjectTypeCheckMark error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_FillSign_Release(fillsign);
      return 1;
    }
    FSDK_FillSignObject_Release(fillsignobject_checkmark);

    point.x = 200;
    point.y = 200;
    error_code = FSDK_FillSign_AddObject(fillsign, e_FSFillSignObjectTypeRoundRectangle, point, 100, 50, e_FSRotation0, &fillsignobject_roundrectangle);
    if (error_code != e_FSErrSuccess) {
      printf("FSFillSignObjectTypeRoundRectangle error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_FillSign_Release(fillsign);
      return 1;
    }
    FSDK_FillSignObject_Release(fillsignobject_roundrectangle);

    point.x = 300;
    point.y = 300;
    error_code = FSDK_FillSign_AddObject(fillsign, e_FSFillSignObjectTypeLine, point, 100, 50, e_FSRotation0, &fillsignobject_typeline);
    if (error_code != e_FSErrSuccess) {
      printf("FSFillSignObjectTypeLine error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_FillSign_Release(fillsign);
      return 1;
    }
    FSDK_FillSignObject_Release(fillsignobject_typeline);

    point.x = 400;
    point.y = 400;
    error_code = FSDK_FillSign_AddObject(fillsign, e_FSFillSignObjectTypeDot, point, 100, 100, e_FSRotation0, &fillsignobject_dot);
    if (error_code != e_FSErrSuccess) {
      printf("FSFillSignObjectTypeDot error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_FillSign_Release(fillsign);
      return 1;
    }
    FSDK_FillSignObject_Release(fillsignobject_dot);

    FSDK_Font_Create0(e_FSStdIDHelvetica, &font);
    textState.font = font;
    textState.font_size = 20;

    point.x = 500;
    point.y = 0;
    textState.origin_position = point;
    textState.charspace = 1.0f;

    FSDK_WStr_Init(&tempstr);
    tempstr.str = L"666777888";
    tempstr.len = wcslen(tempstr.str);
    textdata.text = tempstr;
    textdata.text_state = textState;

    point.x = 400;
    point.y = 100;
    error_code = FSDK_FillSign_AddTextObject(fillsign, &textdata,1, point, 200, 200, e_FSRotation0, FALSE, &text);

    error_code = FSDK_FillSignObject_GenerateContent(text, &isfinish);
    FSDK_FillSignObject_Release(text);

    point.x = 300;
    point.y = 100;
    error_code = FSDK_FillSign_AddObject(fillsign, e_FSFillSignObjectTypeSignature, point, 100, 100, e_FSRotation0, &fillsignobject_signature);
    if (error_code != e_FSErrSuccess) {
      printf("FillSignObjectTypeSignature error!\n");
      FSDK_PDFDoc_Release(doc);
      FSDK_FillSign_Release(fillsign);
      return 1;
    }
    
    FSDK_Bitmap_Create(100, 100, e_FSDIBArgb, NULL, 0, &bitmap);
    FSDK_Bitmap_FillRect(bitmap, 0xFFFF0000, NULL);

    FSDK_SignatureFillSignObject_Create(fillsignobject_signature, &signature);
    FSDK_SignatureFillSignObject_SetBitmap(signature, bitmap);
    FSDK_FillSignObject_GenerateContent(fillsignobject_signature, &isfinish);
    FSDK_GraphicsObjects_GenerateContent(page, &isfinish);

    FSDK_SignatureFillSignObject_Release(signature);
    FSDK_Bitmap_Release(bitmap);
    FSDK_FillSignObject_Release(fillsignobject_signature);

    swprintf_s(pdf_new, MAX_FILE_PATH, L"%lsnew_fillsign.pdf", output_directory);
    error_code = FSDK_PDFDoc_SaveAs(doc, pdf_new, e_FSSaveFlagNoOriginal, &isfinish);
    printf("fillsign Finish : All fillsign generated successfully.\n");
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_FillSign_Release(fillsign);
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    FSDK_FillSign_Release(fillsign);
    FSDK_PDFDoc_Release(doc);
  }
  FSDK_Library_Release();
  return err_ret;
}