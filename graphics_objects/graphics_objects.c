// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to create a PDF document,
// insert the text path and image objects into the created PDF document, copy shading objects,
// and save the file with inserted graphics objects.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include <math.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfgraphicsobject_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
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

void AddTextObjects(FS_PDFPAGE_HANDLE page) {
  FS_POSITION position = NULL;
  FS_TEXTOBJECT_HANDLE text_object = NULL;
  FSTextState state;
  FS_POSITION last_position;
  FSRectF rect;
  float width, height, offset_x, offset_y;
  int text_object_charcount, i = 0;
  wchar_t text_objects_info_file[MAX_FILE_PATH];
  FILE* file = NULL;
  FSMatrix matrix;
  FS_BOOL return_result;

  FSDK_GraphicsObjects_GetLastGraphicsObjectPosition(page, e_FSTypeText, &position);

  FSDK_TextObject_Create(&text_object);

  FSDK_GraphicsObject_SetFillColor(text_object, 0xFFFF7F00);
  // Prepare text state

  state.font_size = 80.0f;
  FSDK_Font_Create(L"Simsun", e_FSStylesSmallCap, e_FSCharsetGB2312, 0, &state.font);
  state.textmode = e_FSModeFill;
  state.origin_position.x = 0;
  state.origin_position.y = 0;
  state.wordspace = 0.0f;
  state.charspace = 0.0f;
  state.textmatrix[0] = 1;
  state.textmatrix[1] = 0;
  state.textmatrix[2] = 0;
  state.textmatrix[3] = 1;
  FSDK_TextObject_SetTextState(text_object, page, state, FALSE, 750);

  // Set text.
  FSDK_TextObject_SetText(text_object, L"Foxit Software");

  FSDK_GraphicsObjects_InsertGraphicsObject(page, position, text_object, &last_position);

  FSDK_GraphicsObject_GetRect(text_object, &rect);

  FSDK_PDFPage_GetWidth(page, &width);
  offset_x = (width - (rect.right - rect.left)) / 2;
  FSDK_PDFPage_GetHeight(page, &height);
  offset_y = height * 0.8f - (rect.top - rect.bottom) / 2;

  matrix.a = 1;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = 1;
  matrix.e = offset_x;
  matrix.f = offset_y;

  FSDK_GraphicsObject_Transform(text_object, matrix, FALSE, &return_result);

  // Generate content
  FSDK_GraphicsObjects_GenerateContent(page, &return_result);

  // Clone a text object from the old text object.
  FSDK_GraphicsObject_Clone(text_object, &text_object);

  FSDK_Font_Create0(e_FSStdIDTimes, &state.font);
  state.font_size = 48;
  state.textmode = e_FSModeFillStrokeClip;

  FSDK_TextObject_SetTextState(text_object, page, state, TRUE, 750);
  FSDK_TextObject_SetText(text_object, L"www.foxitsoftware.com");

  FSDK_GraphicsObject_SetFillColor(text_object, 0xFFAAAAAA);
  FSDK_GraphicsObject_SetStrokeColor(text_object, 0xFFF68C21);
  FSDK_GraphicsObjects_InsertGraphicsObject(page, last_position, text_object, last_position);

  FSDK_GraphicsObject_GetRect(text_object, &rect);
  FSDK_PDFPage_GetWidth(page, &width);
  FSDK_PDFPage_GetHeight(page, &height);
  offset_x = (width - (rect.right - rect.left)) / 2;
  offset_y = height * 0.618f - (rect.top - rect.bottom) / 2;
  matrix.e = offset_x;
  matrix.f = offset_y;
  FSDK_GraphicsObject_Transform(text_object, matrix, FALSE, &return_result);

  // Generate content again after transformation.
  FSDK_GraphicsObjects_GenerateContent(page, &return_result);

  //Show how to get the characters' information of text object.
  FSDK_TextObject_GetCharCount(text_object, &text_object_charcount);

  swprintf_s(text_objects_info_file, MAX_FILE_PATH, L"%lsgraphics_objects/text_objects_info.txt", output_path);

  _wfopen_s(&file, text_objects_info_file, L"wb+");
  
  WriteByteString(file, "The new text object has %d characters.\n", text_object_charcount);

  for (i = 0; i < text_object_charcount; i++) {
    //The character's position.
    FSPointF char_pos;
    float width = 0.0f;
    float height = 0.0f;
    FSDK_TextObject_GetCharPos(text_object, i, &char_pos);
    //The character's width.
    FSDK_TextObject_GetCharWidthByIndex(text_object, i, &width);
    //The character's height.
    FSDK_TextObject_GetCharHeightByIndex(text_object, i, &height);

    WriteByteString(file, "The position of the %d characters is (%f, %f).The width and height of the characters is (%f, %f).\n", i, char_pos.x, char_pos.y, width, height);
  }
  fclose(file);
}

void AddImageObjects(FS_PDFPAGE_HANDLE page, const wchar_t* image_file) {
  FS_POSITION position;
  FS_IMAGE_HANDLE image = NULL;
  FS_IMAGEOBJECT_HANDLE image_object = NULL;
  FS_PDFDOC_HANDLE doc = NULL;
  int width = 0, height = 0;
  float page_width = 0.0f;
  float page_height = 0.0f;
  FSMatrix matrix;
  FS_BOOL return_value;

  FSDK_GraphicsObjects_GetLastGraphicsObjectPosition(page, e_FSTypeImage, &position);

  FSDK_Image_Create0(image_file, &image);

  FSDK_PDFPage_GetDocument(page, &doc);
  FSDK_ImageObject_Create(doc, &image_object);
  FSDK_ImageObject_SetImage(image_object, image, 0);

  FSDK_Image_GetWidth(image, &width);
  FSDK_Image_GetHeight(image, &height);

  FSDK_PDFPage_GetWidth(page, &page_width);

  FSDK_PDFPage_GetHeight(page, &page_height);

  // Please notice the matrix value.
  matrix.a = (float)(width);
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = (float)height;
  matrix.e = (page_width - width) / 2.0f;
  matrix.f = (page_height - height) / 2.0f;
  FSDK_GraphicsObject_SetMatrix(image_object, matrix);

  FSDK_GraphicsObjects_InsertGraphicsObject(page, position, image_object, &position);
  FSDK_GraphicsObjects_GenerateContent(page, &return_value);
}

void AddPieces(FS_PDFPAGE_HANDLE page, FS_SHADINGOBJECT_HANDLE orignal_pieces, FSRectF* dst_rect) {
  FS_POSITION position = NULL;
  FS_SHADINGOBJECT_HANDLE pieces = NULL;
  float a, d, e, f = 0.0f; 
  FSRectF piece_rect;
  FSMatrix matrix;
  FS_BOOL return_result;

  FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeAll, &position);

  FSDK_GraphicsObject_Clone(orignal_pieces, &pieces);

  FSDK_GraphicsObject_GetRect(pieces, &piece_rect);

  // Calculates the transformation matrix between dst_rect and  piece_rect.
  a = (dst_rect->right - dst_rect->left) / (piece_rect.right - piece_rect.left);
  d = (dst_rect->top - dst_rect->bottom) / (piece_rect.top - piece_rect.bottom);
  e = dst_rect->left - piece_rect.left * a;
  f = dst_rect->top - piece_rect.top * d;

  // Transform rect.
  matrix.a = a;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = d;
  matrix.e = e;
  matrix.f = f;

  FSDK_GraphicsObject_Transform(pieces, matrix, FALSE, &return_result);
  FSDK_GraphicsObjects_InsertGraphicsObject(page, position, pieces, &position);

  FSDK_GraphicsObjects_GenerateContent(page, &return_result);
}

void AddPathObjects(FS_PDFPAGE_HANDLE page, FS_SHADINGOBJECT_HANDLE black_pieces, FS_SHADINGOBJECT_HANDLE white_pieces) {
  FS_POSITION position = NULL;
  FS_PATHOBJECT_HANDLE path_object = NULL;
  FS_PATH_HANDLE path = NULL;
  float page_width = 0.0f;
  float page_height = 0.0f;
  float width = 0.0f;
  float start_x = 0.0f;
  float start_y = 0.0f;
  int star[3] = { 3, 9, 15 };
  int i = 0, j = 0, k = 0;
  FS_BOOL return_result = FALSE;
  FSPointF pieces_vector[2][8];

  FSDK_GraphicsObjects_GetLastGraphicsObjectPosition(page, e_FSTypePath, &position);

  FSDK_PathObject_Create(&path_object);
  FSDK_Path_Create(&path);

  FSDK_PDFPage_GetWidth(page, &page_width);

  FSDK_PDFPage_GetHeight(page, &page_height);

  width = min(page_width, page_height) / 20.0f;
  start_x = (page_width - width * 18.0f) / 2.0f;
  start_y = (page_height - width * 18.0f) / 2.0f;

  // Draw a chess board
  for (i = 0; i < 19; i++) {
    float x1 = start_x;
    float y1 = i * width + start_y;
    float x2 = start_x + 18 * width;
    FSPointF pointf;
    FS_BOOL is_move;
    float y2 = 0.0f;

    pointf.x = x1;
    pointf.y = y1;

    FSDK_Path_MoveTo(path, pointf, &is_move);
    pointf.x = x2;
    FSDK_Path_LineTo(path, pointf, &is_move);

    x1 = i * width + start_x;
    y1 = start_y;

    y2 = 18 * width + start_y;
    pointf.x = x1;
    pointf.y = y1;
    FSDK_Path_MoveTo(path, pointf, &is_move);
    pointf.y = y2;
    FSDK_Path_LineTo(path, pointf, &is_move);
  }

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      FSRectF rect;
      FS_BOOL return_value = FALSE;
      rect.left = start_x + star[i] * width - width / 12;
      rect.bottom = start_y + star[j] * width - width / 12;
      rect.right = start_x + star[i] * width + width / 12;
      rect.top = start_y + star[j] * width + width / 12;

      FSDK_Path_AppendEllipse(path, rect, &return_value);
    }
  }
  FSDK_PathObject_SetPathData(path_object, path);

  FSDK_GraphicsObject_SetFillColor(path_object, 0xFF000000);
  FSDK_PathObject_SetFillMode(path_object, e_FSFillModeAlternate);
  FSDK_PathObject_SetStrokeState(path_object, TRUE);
  FSDK_GraphicsObject_SetStrokeColor(path_object, 0xFF000000);

  FSDK_GraphicsObjects_InsertGraphicsObject(page, position, path_object, &position);
  FSDK_GraphicsObjects_GenerateContent(page, &return_result);

  // Draw pieces
  pieces_vector[0][0].x = 3;
  pieces_vector[0][0].y = 3;
  pieces_vector[0][1].x = 3;
  pieces_vector[0][1].y = 7;
  pieces_vector[0][2].x = 3;
  pieces_vector[0][2].y = 15;
  pieces_vector[0][3].x = 13;
  pieces_vector[0][3].y = 2;
  pieces_vector[0][4].x = 13;
  pieces_vector[0][4].y = 16;
  pieces_vector[0][5].x = 13;
  pieces_vector[0][5].y = 17;
  pieces_vector[0][6].x = 15;
  pieces_vector[0][6].y = 16;
  pieces_vector[0][7].x = 16;
  pieces_vector[0][7].y = 16;
  pieces_vector[1][0].x = 11;
  pieces_vector[1][0].y = 16;
  pieces_vector[1][1].x = 12;
  pieces_vector[1][1].y = 14;
  pieces_vector[1][2].x = 14;
  pieces_vector[1][2].y = 4;
  pieces_vector[1][3].x = 14;
  pieces_vector[1][3].y = 15;
  pieces_vector[1][4].x = 15;
  pieces_vector[1][4].y = 3;
  pieces_vector[1][5].x = 15;
  pieces_vector[1][5].y = 9;
  pieces_vector[1][6].x = 15;
  pieces_vector[1][6].y = 15;
  pieces_vector[1][7].x = 16;
  pieces_vector[1][7].y = 15;
  for (k = 0; k < 2; k++) {
    for (i = 0; i < sizeof(pieces_vector[k]) / sizeof(pieces_vector[k][0]); i++) {
      int x = (int)(pieces_vector[k][i].x);
      int y = (int)(pieces_vector[k][i].y);
      FSRectF rect;
      rect.left = start_x + x * width - width / 2.f;
      rect.bottom = start_y + y * width - width / 2.f;
      rect.right = start_x + x * width + width / 2.f;
      rect.top = start_y + y * width + width / 2.f;
      AddPieces(page, k % 2 ? white_pieces : black_pieces, &rect);
    }
  }
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t input_file[MAX_FILE_PATH];
  FS_BOOL is_generate;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsgraphics_objects/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsgraphics_objects.pdf", input_path);

  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_PDFPAGE_HANDLE original_page = NULL;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FS_POSITION position = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    wchar_t image_file[MAX_FILE_PATH];
    FS_BOOL is_save;
    wchar_t output_file[MAX_FILE_PATH];
    FS_SHADINGOBJECT_HANDLE black_pieces = NULL;
    FS_SHADINGOBJECT_HANDLE white_pieces = NULL;

    FSDK_PDFDoc_Create0(input_file, &doc);
    error_code = FSDK_PDFDoc_Load(doc, NULL);

    if (error_code != e_FSErrSuccess) {
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
      return 1;
    }

    // Get original shading objects from PDF page.
    FSDK_PDFDoc_GetPage(doc, 0, &original_page);
    FSDK_PDFPage_StartParse(original_page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);
    FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(original_page, e_FSTypeShading, &position);
    if (!position)
      return 1;

    FSDK_GraphicsObjects_GetGraphicsObject(original_page, position, &black_pieces);
    FSDK_GraphicsObjects_GetNextGraphicsObjectPosition(original_page, position, e_FSTypeShading, &position);

    FSDK_GraphicsObjects_GetGraphicsObject(original_page, position, &white_pieces);

    // Add a new PDF page and insert text objects.
    FSDK_PDFDoc_InsertPage0(doc, 0, e_FSSizeLetter, &page);
    AddTextObjects(page);

    // Add a new PDF page and insert image objects.
    FSDK_PDFDoc_InsertPage0(doc, 1, e_FSSizeLetter, &page);

    swprintf_s(image_file, MAX_FILE_PATH, L"%lssdk.png", input_path);

    AddImageObjects(page, image_file);

    FSDK_PDFPage_Release(page);

    // Add a new PDF page and insert path objects, and copy shading objects.
    FSDK_PDFDoc_InsertPage0(doc, 2, e_FSSizeLetter, &page);
    AddPathObjects(page, black_pieces, white_pieces);
    FSDK_PDFPage_Release(page);

    FSDK_PDFPage_Release(original_page);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsgraphics_objects.pdf", output_directory);

    FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagsSaveFlagNormal, &is_save);

    printf("Add graphics objects to PDF file.\n");;

    // Remove text objects from PDF page.
    FSDK_PDFDoc_GetPage(doc, 0, &page);
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);
    FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeAll, &position);
    while (position) {
      FS_GRAPHICSOBJECT_HANDLE obj = NULL;
      FSGraphicsObjectType obj_type;

      FSDK_GraphicsObjects_GetGraphicsObject(page, position, &obj);

      FSDK_GraphicsObject_GetType(obj, &obj_type);
      if (obj_type == e_FSTypeText) {
        FS_TEXTOBJECT_HANDLE textobj = obj;
        FS_WSTR s;
        FSDK_WStr_Init(&s);
        FSDK_TextObject_GetText(textobj, &s);
        if (s.len > 0) {
          FS_BOOL is_remove;
          FSDK_GraphicsObjects_RemoveGraphicsObject(page, textobj, &is_remove);
          FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeAll, &position);
          FSDK_WStr_Clear(&s);
          continue;
        }
      } else if (obj_type == e_FSTypeFormXObject) {
        FS_FORMXOBJECT_HANDLE formxobj = obj;
        FS_GRAPHICSOBJECTS_HANDLE graphicsObjects_form = NULL;
        FS_POSITION pos = NULL;

        FSDK_FormXObject_GetGraphicsObjects(formxobj, &graphicsObjects_form);

        FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(graphicsObjects_form, e_FSTypeText, &pos);
        if (pos) {
          FS_TEXTOBJECT_HANDLE textobj = NULL;
          FSGraphicsObjectType type;

          FSDK_GraphicsObjects_GetGraphicsObject(page, pos, &textobj);

          FSDK_GraphicsObject_GetType(textobj, &type);
          if (type == e_FSTypeText) {
            FS_WSTR s;
            FSDK_WStr_Init(&s);
            FSDK_TextObject_GetText(textobj, &s);
            if (s.len > 0) {
              FS_BOOL is_remove;
              FSDK_GraphicsObjects_RemoveGraphicsObject(page, textobj, &is_remove);
              FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeAll, &position);
              FSDK_WStr_Clear(&s);
              continue;
            }
          }
        }
      }
      FSDK_GraphicsObjects_GetNextGraphicsObjectPosition(page, position, e_FSTypeAll, &position);
    }
    FSDK_GraphicsObjects_GenerateContent(page, &is_generate);
    FSDK_PDFPage_Release(page);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsgraphics_objects_remove_text_objects.pdf", output_directory);
    FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagsSaveFlagNormal, &is_save);
    printf("Remove text objects from PDF file.\n");

    FSDK_PDFDoc_Release(doc);
  }

  FSDK_Library_Release();
  return err_ret;
}
