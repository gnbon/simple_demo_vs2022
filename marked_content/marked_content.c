// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to mark PDF content and
// get marked content from graphics objects.

// Include Foxit SDK header files.
#include <time.h>
#include<stdarg.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_image_c.h"
#include "../../../include/fs_pdfgraphicsobject_c.h"
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

wchar_t output_directory[MAX_FILE_PATH];

void WriteByteString(FILE* file, const char* format, ...) {
  va_list vars;
  if (file) {
    // Get variable list
    va_start(vars, format);
    // Call vfprintf to format log data and output to log file
    vfprintf(file, format, vars);
    // End variable list
    va_end(vars);
  }
}

void CreateMarkedContent(FS_PDFPAGE_HANDLE page) {
  FS_POSITION position;
  FS_TEXTOBJECT_HANDLE text_object = NULL;
  FSTextState state;
  FSRectF rect;
  float width, height;
  float offset_x, offset_y;
  FSMatrix matrix;
  FS_BOOL return_result;
  FS_MARKEDCONTENT_HANDLE content = NULL;
  FS_PDFDICTIONARY_HANDLE span_dict = NULL;
  int value;
  FS_BOOL return_value;

  FSDK_GraphicsObjects_GetLastGraphicsObjectPosition(page, e_FSTypeText, &position);

  FSDK_TextObject_Create(&text_object);

  FSDK_GraphicsObject_SetFillColor(text_object, 0xFF83AF9B);

  // Prepare text state
  state.font_size = 72.f;
  FSDK_Font_Create0(e_FSStdIDTimesB, &state.font);
  state.textmode = e_FSModeFill;
  state.origin_position.x = 0;
  state.origin_position.y = 0;
  state.wordspace = 0.0f;
  state.charspace = 0.0f;
  state.textmatrix[0] = 1;
  state.textmatrix[1] = 0;
  state.textmatrix[2] = 0;
  state.textmatrix[3] = 1;
  state.version = 1;

  FSDK_TextObject_SetTextState(text_object, page, state, FALSE, 750);

  FSDK_TextObject_SetText(text_object, L"Marked-Content");
  FSDK_GraphicsObjects_InsertGraphicsObject(page, position, text_object, &position);

  FSDK_GraphicsObject_GetRect(text_object, &rect);

  FSDK_PDFPage_GetWidth(page, &width);
  offset_x = (width - (rect.right - rect.left)) / 2;
  FSDK_PDFPage_GetHeight(page, &height);
  offset_y = height * 0.618f - (rect.top - rect.bottom) / 2;

  matrix.a = 1;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = 1;
  matrix.e = offset_x;
  matrix.f = offset_y;

  FSDK_GraphicsObject_Transform(text_object, matrix, FALSE, &return_result);

  FSDK_GraphicsObject_GetMarkedContent(text_object, &content);

  FSDK_PDFDictionary_Create(&span_dict);
  FSDK_PDFDictionary_SetAtInteger(span_dict, "Direct", 55);

  FSDK_MarkedContent_AddItem(content, "Span", span_dict, &value);
  FSDK_PDFObject_Release(span_dict);
  // Generate content
  FSDK_GraphicsObjects_GenerateContent(page, &return_value);
}

FS_BOOL GetMarkedContentObject(const wchar_t* pdf_file) {
  FS_PDFDOC_HANDLE doc = NULL;
  FSErrorCode ret = e_FSErrSuccess;
  FS_PDFPAGE_HANDLE page = NULL;
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  FS_POSITION position;
  FS_TEXTOBJECT_HANDLE text_obj = NULL;
  FS_MARKEDCONTENT_HANDLE content = NULL;
  int item_count, i = 0;
  wchar_t file_info[MAX_FILE_PATH];
  FILE* file = NULL;

  FSDK_PDFDoc_Create0(pdf_file, &doc);
  ret = FSDK_PDFDoc_Load(doc, NULL);
  if (ret != e_FSErrSuccess) {
    wprintf(L"[Failed] Cannot load PDF document %ls.\r\n Error message:%d\r\n", pdf_file, ret);
    return FALSE;
  }

  FSDK_PDFDoc_GetPage(doc, 0, &page);

  FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);

  FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeText, &position);

  FSDK_GraphicsObjects_GetGraphicsObject(page, position, &text_obj);

  FSDK_GraphicsObject_GetMarkedContent(text_obj, &content);
  FSDK_MarkedContent_GetItemCount(content, &item_count);

  swprintf_s(file_info, MAX_FILE_PATH, L"%lsmarkedcontent.txt", output_directory);

  _wfopen_s(&file, file_info, L"w+b");

  // Get marked content property
  for (i = 0; i < item_count; i++) {
    FS_BSTR tag_name;
    int mcid;
    WriteByteString(file, "index: %d\r\n", i);
    FSDK_BStr_Init(&tag_name);
    FSDK_MarkedContent_GetItemTagName(content, i, &tag_name);
    WriteByteString(file, "Tag name: %s\r\n", (const char*)tag_name.str);

    FSDK_MarkedContent_GetItemMCID(content, i, &mcid);
    WriteByteString(file, "mcid: %d\r\n", mcid);
    FSDK_BStr_Clear(&tag_name);
  }
  return TRUE;
}

int main(int argc, char *argv[]) {
  FSErrorCode error_code = e_FSErrSuccess;
  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsmarked_content/", output_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    wchar_t output_file[MAX_FILE_PATH];
    FS_BOOL is_save;

    FSDK_PDFDoc_Create(&doc);

    FSDK_PDFDoc_InsertPage0(doc, 0, e_FSSizeLetter, &page);

    CreateMarkedContent(page);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsmarked_content.pdf", output_directory);
    FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagsSaveFlagNormal, &is_save);

    if (!GetMarkedContentObject(output_file)) {
      return 1;
    }

    FSDK_PDFPage_Release(page);
    FSDK_PDFDoc_Release(doc);
    printf("Add marked content to PDF file.\n");
  }

  FSDK_Library_Release();
  printf("marked content demo done.\n");
  return 0;
}
