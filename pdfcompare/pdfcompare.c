// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to compare pdf page with the other.

#include <time.h>
#include<direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_search_c.h"
#include "../../../include/fs_compare_c.h"
#include "../../../include/fx_basic_c.h"
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

FSDateTime GetLocalDateTime() {
  time_t t = time(NULL);
  struct tm _Tm;
  struct tm* rime = NULL;
  long time_zone = 0;
  int timezone_hour, timezone_minute = 0;
  FSDateTime datetime;

  localtime_s(&_Tm, &t);
  rime = &_Tm;
  _tzset();

  _get_timezone(&time_zone);
  timezone_hour = time_zone / 3600 * -1;
  timezone_minute = (abs(time_zone) % 3600) / 60;

  datetime.year = (FS_UINT16)(rime->tm_year + 1900);
  datetime.month = (FS_UINT16)(rime->tm_mon + 1);
  datetime.day = (FS_UINT16)(rime->tm_mday);
  datetime.hour = (FS_UINT16)(rime->tm_hour);
  datetime.minute = (FS_UINT16)(rime->tm_min);
  datetime.second = (FS_UINT16)(rime->tm_sec);
  datetime.milliseconds = 0;
  datetime.utc_hour_offset = timezone_hour;
  datetime.utc_minute_offset = timezone_minute;

  return datetime;
}

void RandomUID(wchar_t* uuid) {
  const wchar_t* c = L"0123456789qwertyuiopasdfghjklzxcvbnm";
  int n = 0;

  for (n = 0; n < 16; n++) {
    wchar_t uuid_temp[60];
    int b = rand() % 255;
    switch (n) {
    case 6:
      swprintf_s(uuid_temp, 60, L"4%x", b % 15);
      break;
    case 8:
      swprintf_s(uuid_temp, 60, L"%c%x", c[rand() % wcslen(c)], b % 15);
      break;
    default:
      swprintf_s(uuid_temp, 60, L"%02x", b);
      break;
    }
    swprintf(uuid, 60, L"%ls", uuid_temp);

    switch (n) {
    case 3:
    case 5:
    case 7:
    case 9:
      swprintf(uuid, 60, L"%s-", uuid);
      break;
    }
  }
}

void CreateInsertStamp(FS_PDFPAGE_HANDLE page, FSRectF *rects, int length, FS_RGB color, const wchar_t* cs_contents, const wchar_t* cs_type, const wchar_t* cs_obj_type) {
  FSRectF rc_stamp;
  int rect_size = length;
  FS_IMAGE_HANDLE image = NULL;
  FS_STAMPANNOT_HANDLE stamp = NULL;
  wchar_t uuid[MAX_FILE_PATH];
  FS_BOOL return_value;

  if (rect_size > 0) {
    FSRectF item = rects[0];
    rc_stamp.left = item.left;
    rc_stamp.top = item.top - 4;
    rc_stamp.right = rc_stamp.left + 4;
    rc_stamp.bottom = rc_stamp.top - 8;
  }

  FSDK_Image_Create0(L"../input_files/pdfcompare/insert_stamp.png", &image);
  FSDK_PDFPage_AddAnnot(page, e_FSStamp, rc_stamp, &stamp);
  FSDK_Annot_SetContent(stamp, cs_contents);
  FSDK_Annot_SetBorderColor(stamp, color);
  FSDK_Markup_SetSubject(stamp, cs_obj_type);
  FSDK_Markup_SetTitle(stamp, cs_type);
  FSDK_Markup_SetCreationDateTime(stamp, GetLocalDateTime());
  FSDK_Annot_SetModifiedDateTime(stamp, GetLocalDateTime());

  RandomUID(uuid);
  FSDK_Annot_SetUniqueID(stamp, uuid);
  FSDK_Stamp_SetImage(stamp, image, 0, 0);

  FSDK_Annot_ResetAppearanceStream(stamp, &return_value);
  FSDK_Annot_Release(stamp);
  FSDK_Image_Release(image);
}

void CreateSquigglyRect(FS_PDFPAGE_HANDLE page, FSRectF *rects, int length, FS_RGB color, const wchar_t* cs_contents, const wchar_t* cs_yype, const wchar_t* cs_obj_type) {
  FS_ANNOT_HANDLE annot = NULL;
  FSRectF rect;
  FS_SQUIGGLYANNOT_HANDLE squiggly = NULL;
  FSQuadPoints *quad_points_array = NULL;
  int rect_size = 0, i = 0;
  FS_BOOL return_code;
  wchar_t uuid[MAX_FILE_PATH];

  if (length == 0 || rects == NULL) return;

  rect.left = rect.right = rect.top = rect.bottom = 0.0f;
  FSDK_PDFPage_AddAnnot(page, e_FSSquiggly, rect, &annot);

  FSDK_Squiggly_Create0(annot, &squiggly);
  FSDK_Annot_SetContent(squiggly, cs_contents);

  quad_points_array = (FSQuadPoints*)malloc(sizeof(FSQuadPoints) * length);
  rect_size = length;
  for (i = 0; i < rect_size; i++) {
    FSRectF item = rects[i];
    FSPointF point2;
    FSQuadPoints quad_points;
    FSPointF point;
    FSPointF point3;
    FSPointF point4;

    point.x = item.left;
    point.y = item.top;
    quad_points.first = point;

    point2.x = item.right;
    point2.y = item.top;
    quad_points.second = point2;

    point3.x = item.left;
    point3.y = item.bottom;
    quad_points.third = point3;

    point4.x = item.right;
    point4.y = item.bottom;
    quad_points.fourth = point4;
    quad_points_array[i] = quad_points;
  }
  FSDK_TextMarkup_SetQuadPoints(squiggly, quad_points_array, length);
  free(quad_points_array);
  FSDK_Annot_SetBorderColor(squiggly, color);
  FSDK_Markup_SetSubject(squiggly, cs_obj_type);
  FSDK_Markup_SetTitle(squiggly, cs_yype);
  FSDK_Markup_SetCreationDateTime(squiggly, GetLocalDateTime());
  FSDK_Annot_SetModifiedDateTime(squiggly, GetLocalDateTime());

  RandomUID(uuid);
  FSDK_Annot_SetUniqueID(squiggly, uuid);

  FSDK_Annot_ResetAppearanceStream(squiggly, &return_code);
  FSDK_Squiggly_Release(squiggly);
  FSDK_Annot_Release(annot);
}

void CreateDeleteTextStamp(FS_PDFPAGE_HANDLE page, FSRectF* rects, int length, FS_RGB color, const wchar_t* cs_contents, const wchar_t* cs_type, const wchar_t* cs_obj_type) {
  FSRectF rc_stamp;
  int rect_size = 0;
  wchar_t input_path_new[MAX_FILE_PATH];
  FS_IMAGE_HANDLE image = NULL;
  FS_STAMPANNOT_HANDLE stamp = NULL;
  FS_ANNOT_HANDLE annot = NULL;
  wchar_t uuid[MAX_FILE_PATH];
  FS_BOOL return_code;

  if (length == 0 || rects == NULL) return;

  rect_size = length;
  if (rect_size > 0) {
    FSRectF item = rects[0];
    rc_stamp.left = item.left;
    rc_stamp.top = item.top + 12;
    rc_stamp.right = rc_stamp.left + 9;
    rc_stamp.bottom = rc_stamp.top - 12;
  }

  swprintf(input_path_new, MAX_FILE_PATH, L"%lspdfcompare/delete_stamp.png", input_path);

  FSDK_Image_Create0(input_path_new, &image);

  FSDK_PDFPage_AddAnnot(page, e_FSStamp, rc_stamp, &annot);
  FSDK_Stamp_Create0(annot, &stamp);
  FSDK_Annot_SetContent(stamp, cs_contents);
  FSDK_Annot_SetBorderColor(stamp, color);
  FSDK_Markup_SetSubject(stamp, cs_obj_type);
  FSDK_Markup_SetTitle(stamp, cs_type);
  FSDK_Markup_SetCreationDateTime(stamp, GetLocalDateTime());
  FSDK_Annot_SetModifiedDateTime(stamp, GetLocalDateTime());

  RandomUID(uuid);
  FSDK_Annot_SetUniqueID(stamp, uuid);
  FSDK_Stamp_SetImage(stamp, image, 0, 0);

  FSDK_Annot_ResetAppearanceStream(stamp, &return_code);
  FSDK_Image_Release(image);
  FSDK_Stamp_Release(stamp);
  FSDK_Annot_Release(annot);
}

void CreateDeleteText(FS_PDFPAGE_HANDLE page, FSRectF *rects, int length, FS_RGB color, const wchar_t* cs_contents, const wchar_t*  cs_type, const wchar_t*  cs_obj_type) {
  FS_STRIKEOUTANNOT_HANDLE strikeout = NULL;
  FS_ANNOT_HANDLE annot = NULL;
  FSRectF rectf;
  FSQuadPoints *quad_points_array = NULL;
  int rect_size = length, i = 0;
  wchar_t uuid[MAX_FILE_PATH];
  FS_BOOL return_code;

  rectf.left = rectf.bottom  = rectf.right = rectf.top = 0.0f;
  FSDK_PDFPage_AddAnnot(page, e_FSStrikeOut, rectf, &annot);
  FSDK_StrikeOut_Create0(annot, &strikeout);
  FSDK_Annot_SetContent(strikeout, cs_contents);

  quad_points_array = (FSQuadPoints*)malloc(sizeof(FSQuadPoints) * length);
  
  for (i = 0; i < rect_size; i++) {
    FSRectF item = rects[i];
    FSQuadPoints quad_points;
    FSPointF point;
    FSPointF point2;
    FSPointF point3;
    FSPointF point4;

    point.x = item.left;
    point.y = item.top;
    quad_points.first = point;

    point2.x = item.right;
    point2.y = item.top;
    quad_points.second = point2;

    point3.x = item.left;
    point3.y = item.bottom;
    quad_points.third = point3;

    point4.x = item.right;
    point4.y = item.bottom;
    quad_points.fourth = point4;
    quad_points_array[i] = quad_points;
  }
  FSDK_TextMarkup_SetQuadPoints(strikeout, quad_points_array, length);
  free(quad_points_array);

  FSDK_Annot_SetBorderColor(strikeout, color);
  FSDK_Markup_SetSubject(strikeout, cs_obj_type);
  FSDK_Markup_SetTitle(strikeout, cs_type);
  FSDK_Markup_SetCreationDateTime(strikeout, GetLocalDateTime());
  FSDK_Annot_SetModifiedDateTime(strikeout, GetLocalDateTime());

  RandomUID(uuid);
  FSDK_Annot_SetUniqueID(strikeout, uuid);

  FSDK_Annot_ResetAppearanceStream(strikeout, &return_code);
  FSDK_StrikeOut_Release(strikeout);
  FSDK_Annot_Release(annot);
}

int main(int argc, char *argv[]) {
  wchar_t output_directory[MAX_FILE_PATH];
  int err_ret = 0;
  wchar_t input_base_file[MAX_FILE_PATH];
  wchar_t input_compared_file[MAX_FILE_PATH];
  FSErrorCode error_code = e_FSErrSuccess;
  FS_PDFDOC_HANDLE base_doc = NULL;
  FS_PDFDOC_HANDLE compared_doc = NULL;
  FS_COMPARISON_HANDLE comparison = NULL;
  FSCompareResults result;
  FS_PDFPAGE_HANDLE page1 = NULL;
  FS_PDFPAGE_HANDLE page = NULL;
  FS_UINT32 i = 0;
  FS_BOOL return_result = FALSE;
  wchar_t output_base_doc[MAX_FILE_PATH];
  wchar_t output_compared_doc[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE new_doc = NULL;
  wchar_t output_new_doc[MAX_FILE_PATH];

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdfcompare/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_base_file, MAX_FILE_PATH, L"%lspdfcompare/test_base.pdf", input_path);
  swprintf_s(input_compared_file, MAX_FILE_PATH, L"%lspdfcompare/test_compared.pdf", input_path);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  error_code = FSDK_PDFDoc_Create0(input_base_file, &base_doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(base_doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(base_doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_base_file, error_code);
    return 1;
  }

  error_code = FSDK_PDFDoc_Create0(input_compared_file, &compared_doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(compared_doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(compared_doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_compared_file, error_code);
    return 1;
  }

  error_code = FSDK_Comparison_Create(base_doc, compared_doc, &comparison);
  result.base_doc_results_array = NULL;
  result.compared_doc_results_array = NULL;
  error_code = FSDK_Comparison_DoCompare(comparison, 0, 0, e_FSCompareTypeText, &result);
  result.base_doc_results_array = malloc(sizeof(FSCompareResultInfo) * result.base_doc_results_array_length);
  for (i = 0; i < result.base_doc_results_array_length; i++)
    result.base_doc_results_array[i].rect_array = NULL;
  result.compared_doc_results_array = (FSCompareResultInfo*)malloc(sizeof(FSCompareResultInfo) * result.compared_doc_results_array_length);
  for (i = 0; i < result.compared_doc_results_array_length; i++)
    result.compared_doc_results_array[i].rect_array = NULL;
  error_code = FSDK_Comparison_DoCompare(comparison, 0, 0, e_FSCompareTypeText, &result);
  for (i = 0; i < result.base_doc_results_array_length; i++)
    result.base_doc_results_array[i].rect_array = (FSRectF*)malloc(sizeof(FSRectF) * result.base_doc_results_array[i].array_length_rect_array);
  for (i = 0; i < result.compared_doc_results_array_length; i++)
    result.compared_doc_results_array[i].rect_array = (FSRectF*)malloc(sizeof(FSRectF) * result.compared_doc_results_array[i].array_length_rect_array);
  error_code = FSDK_Comparison_DoCompare(comparison, 0, 0, e_FSCompareTypeText, &result);

  error_code = FSDK_PDFDoc_GetPage(base_doc, 0, &page1);

  error_code = FSDK_PDFDoc_GetPage(compared_doc, 0, &page);
  for (i = 0; i < result.base_doc_results_array_length; i++) {
    FSCompareResultInfo item = result.base_doc_results_array[i];
    FSCompareResultType type = item.type;
    if (type == e_FSCompareResultTypeDeleteText) {
      wchar_t res_string_new[100];
      swprintf_s(res_string_new, 100, L"\"%ls\"", item.diff_contents.str);
      CreateDeleteText(page1, item.rect_array, item.array_length_rect_array, 0xff0000, res_string_new, L"Compare : Delete", L"Text");
    } else if (type == e_FSCompareResultTypeInsertText) {
      wchar_t res_string_new[100];
      swprintf_s(res_string_new, 100, L"\"%ls\"", item.diff_contents.str);
      CreateInsertStamp(page1, item.rect_array, item.array_length_rect_array, 0x0000ff, res_string_new, L"Compare : Insert", L"Text");
    }
    else if (type == e_FSCompareResultTypeReplaceText) {
      wchar_t res_string_new[100];
      swprintf_s(res_string_new, 100, L"[New]: \"%ls\"\r\n[Old]: \"%ls\"", result.compared_doc_results_array[i].diff_contents.str, item.diff_contents.str);
      CreateSquigglyRect(page1, item.rect_array, item.array_length_rect_array, 0xe7651a, res_string_new, L"Compare : Replace", L"Text");
    }
  }
  for (i = 0; i < result.compared_doc_results_array_length; i++) {
    const FSCompareResultInfo item = result.compared_doc_results_array[i];
    FSCompareResultType type = item.type;
    if (type == e_FSCompareResultTypeDeleteText) {
      wchar_t res_string_new[100];
      swprintf_s(res_string_new, 100, L"\"%ls\"", item.diff_contents.str);
      CreateDeleteTextStamp(page, item.rect_array, item.array_length_rect_array, 0xff0000, res_string_new, L"Compare : Delete", L"Text");
    } else if (type == e_FSCompareResultTypeInsertText) {
      wchar_t res_string_new[100];
      swprintf_s(res_string_new, 100, L"\"%ls\"", item.diff_contents.str);
      CreateDeleteText(page, item.rect_array, item.array_length_rect_array, 0x0000ff, res_string_new, L"Compare : Insert", L"Text");
    }
    else if (type == e_FSCompareResultTypeReplaceText) {
      wchar_t res_string_new[100];
      swprintf_s(res_string_new, 100, L"[Old]: \"%ls\"\r\n[New]: \"%ls\"", item.diff_contents.str, result.base_doc_results_array[i].diff_contents.str);
      CreateSquigglyRect(page, item.rect_array, item.array_length_rect_array, 0xe7651a, res_string_new, L"Compare : Replace", L"Text");
    }
  }

  swprintf_s(output_base_doc, MAX_FILE_PATH, L"%lsold.pdf", output_directory);
  error_code = FSDK_PDFDoc_SaveAs(base_doc, output_base_doc, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(base_doc);
    return 1;
  }

  swprintf_s(output_compared_doc, MAX_FILE_PATH, L"%lsnew.pdf", output_directory);
  error_code = FSDK_PDFDoc_SaveAs(compared_doc, output_compared_doc, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(compared_doc);
    return 1;
  }

  error_code = FSDK_Comparison_GenerateComparedDoc(comparison, e_FSCompareTypeAll, &new_doc);

  swprintf_s(output_new_doc, MAX_FILE_PATH, L"%lsgenerate_result.pdf", output_directory);
  error_code = FSDK_PDFDoc_SaveAs(new_doc, output_new_doc, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(new_doc);
    return 1;
  }
  FSDK_Comparison_Release(comparison);
  FSDK_PDFDoc_Release(base_doc);
  FSDK_PDFDoc_Release(compared_doc);
  FSDK_PDFDoc_Release(new_doc);
  for (i = 0; i < result.base_doc_results_array_length; i++) {
    free(result.base_doc_results_array[i].rect_array);
    FSDK_WStr_Clear(&result.base_doc_results_array[i].diff_contents);
  }
  for (i = 0; i < result.compared_doc_results_array_length; i++) {
    free(result.compared_doc_results_array[i].rect_array);
    FSDK_WStr_Clear(&result.compared_doc_results_array[i].diff_contents);
  }
  free(result.base_doc_results_array);
  free(result.compared_doc_results_array);

  FSDK_Library_Release();
  printf("Pdf compare test.\n");
  return err_ret;
}

