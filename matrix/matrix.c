// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file is a demo to demonstrate how to use matrix to translate, scale... objects.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>
#include <windows.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_pdfobject_c.h"

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
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  wchar_t file_info[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;
  FS_PDFPAGE_HANDLE page = NULL;
  FS_PROGRESSIVE_HANDLE progressive_handle;
  FS_POSITION pos;
  int flag = 0;
  FS_BOOL result;
  wchar_t output_file[MAX_FILE_PATH] = { 0 };

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsmatrix/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsSamplePDF.pdf", input_path);
  swprintf_s(file_info, MAX_FILE_PATH, L"%lsmatrix_info.txt", output_directory);

  // Load a document
  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;

  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    return 1;
  }

  // Get and parse page.
  error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    return 1;
  }

  FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive_handle);
  FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeAll, &pos);

  while(pos) {
    FS_GRAPHICSOBJECT_HANDLE obj = NULL;
    FSGraphicsObjectType type;
    float start_x = 0;
    FS_GRAPHICSOBJECT_HANDLE clone_obj1 = NULL;
    FSMatrix matrix;
    FS_POSITION newpos = NULL;
    FS_GRAPHICSOBJECT_HANDLE clone_obj2 = NULL;
    float d,e,f,distance;
    FS_GRAPHICSOBJECT_HANDLE clone_obj3 = NULL;
    FS_GRAPHICSOBJECT_HANDLE clone_obj4 = NULL;

    if(flag == (2 | 4))
      break;

    FSDK_GraphicsObjects_GetGraphicsObject(page, pos, &obj);
    FSDK_GraphicsObjects_GetNextGraphicsObjectPosition(page, pos, e_FSTypeAll, &pos);

    FSDK_GraphicsObject_GetType(obj, &type);

      // Get one TextObject or one ImageObject.
    if(type == e_FSTypeText && !(flag & 2)) {
      FS_TEXTOBJECT_HANDLE text_object = (FS_TEXTOBJECT_HANDLE)obj;
      FS_WSTR text;
      FSDK_WStr_Init(&text);
      FSDK_TextObject_GetText(text_object, &text);
      if(wcscmp(text.str, L"Foxit Software Incorporated") != 0){
        FSDK_WStr_Clear(&text);
        continue;
      }
      FSDK_WStr_Clear(&text);
      start_x = 400.0f;
      flag |= 2;
    } else if(type == e_FSTypeImage && !(flag & 4)) {
      start_x = 150.0f;
      flag |= 4;
    }else{
      continue;
    }

    // Now we have found the TextObject or the ImageObject, transform them.
    FSDK_GraphicsObject_Clone(obj, &clone_obj1);
    FSDK_GraphicsObject_GetMatrix(clone_obj1, &matrix);
    FSDK_Matrix_Translate(&matrix, 0, 150);
    FSDK_GraphicsObject_SetMatrix(clone_obj1, matrix);
    FSDK_GraphicsObjects_InsertGraphicsObject(page, NULL, clone_obj1, &newpos);

    FSDK_GraphicsObject_Clone(obj, &clone_obj2);
    FSDK_GraphicsObject_GetMatrix(clone_obj2, &matrix);
    d = matrix.d;
    e = matrix.e;
    f = matrix.f;
    // Translate it to the original point(0,0) first
    FSDK_Matrix_Translate(&matrix, -e, -f);
    // Rotate the matrix 90 degree in anticlockwise direction.
    FSDK_Matrix_Rotate(&matrix, 1.57f);
    //Page rotation should be considered, assume that the current page rotation is 0.
    distance = d;
    if(type == e_FSTypeText) {
      FSRectF obj_rect;
      FSDK_GraphicsObject_GetRect(obj, &obj_rect);
      distance = obj_rect.top - obj_rect.bottom;
    }
    // Translate it to the specific coordinate(start_x,400)
    FSDK_Matrix_Translate(&matrix, distance + start_x , 400);
    FSDK_GraphicsObject_SetMatrix(clone_obj2, matrix);
    FSDK_GraphicsObjects_InsertGraphicsObject(page, NULL, clone_obj2, &newpos);

    FSDK_GraphicsObject_Clone(obj, &clone_obj3);
    FSDK_GraphicsObject_GetMatrix(clone_obj3, &matrix);
    // Translate it to the original point(0,0) first
    FSDK_Matrix_Translate(&matrix, -e, -f);
    // Magnify the matrix 1.5 times in both horizontal and vertical directions.
    FSDK_Matrix_Scale(&matrix, 1.5, 1.5);
    // Translate it to the specific coordinate(start_x,600)
    FSDK_Matrix_Translate(&matrix, start_x, 600.0f);
    FSDK_GraphicsObject_SetMatrix(clone_obj3, matrix);
    FSDK_GraphicsObjects_InsertGraphicsObject(page, NULL, clone_obj3, &newpos);

    FSDK_GraphicsObject_Clone(obj, &clone_obj4);
    FSDK_GraphicsObject_GetMatrix(clone_obj4, &matrix);
    // Translate it to the original point(0,0) first
    FSDK_Matrix_Translate(&matrix, -e, -f);
    // Skews the x axis by an angle 0.5 and the y axis by an angle 0.5.
    FSDK_Matrix_Shear(&matrix, 0.5f, 0.5f);
    // Translate it to the specific coordinate(start_x,800)
    FSDK_Matrix_Translate(&matrix, start_x, 800.0f);
    FSDK_GraphicsObject_SetMatrix(clone_obj4, matrix);
    FSDK_GraphicsObjects_InsertGraphicsObject(page, NULL, clone_obj4, &newpos);
  }
  // Generate the page content
  FSDK_GraphicsObjects_GenerateContent(page, &result);

  // Save the pdf document
  swprintf_s(output_file, MAX_FILE_PATH, L"%ls/MatrixTransformResult.pdf", output_directory);
  FSDK_PDFDoc_SaveAs(doc, output_file, 0, &result);
  
  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();
  printf("Matrix demo.\n");
  return err_ret;
}