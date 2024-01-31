// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to add table to PDF document.

// Include Foxit SDK header files.
#include <time.h>
#include<stdarg.h>
#include<direct.h>
#include <stdio.h>
#include <math.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_tablegenerator_c.h"

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

FSDateTime GetLocalDateTime() {
  time_t t = time(NULL);
  struct tm _Tm;
  struct tm* rime = NULL;
  long time_zone = 0;
  int timezone_hour, timezone_minute;
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

void SetTableTextStyle(int index, FSRichTextStyle* style) {
  FS_FONT_HANDLE font0 = NULL;
  FSDK_Font_Create0(e_FSStdIDHelvetica, &font0);
  style->font = font0;
  style->text_size = 10;
  style->text_alignment = e_FSAlignmentLeft;
  style->text_color = 0x000000;
  style->is_bold = FALSE;
  style->is_italic = FALSE;
  style->is_underline = FALSE;
  style->is_strikethrough = FALSE;
  style->mark_style = e_FSCornerMarkNone;

  switch (index) {
  case 1:
    style->text_alignment = e_FSAlignmentCenter;
    break;
  case 2: {
    style->text_alignment = e_FSAlignmentRight;
    style->text_color = 0x00FF00;
    break;
  }
  case 3:
    style->text_size = 15;
    break;
  case 4: {
    FS_FONT_HANDLE font = NULL;
    FSDK_Font_Create(L"Times New Roman", 0, e_FSCharsetANSI, 0, &font);
    style->font = font;
    style->text_color = 0xFF0000;
    style->text_alignment = e_FSAlignmentRight;
    break;
  }
  case 5: {
    FS_FONT_HANDLE font = NULL;
    FSDK_Font_Create(L"Times New Roman", 0, e_FSCharsetANSI, 0, &font);
    style->font = font;
    style->is_bold = TRUE;
    style->text_alignment = e_FSAlignmentRight;
    break;
  }
  case 6: {
    style->is_bold = TRUE;
    style->is_italic = TRUE;
    break;
  }
  case 7: {
    style->is_bold = TRUE;
    style->is_italic = TRUE;
    style->text_alignment = e_FSAlignmentCenter;
    break;
  }
  case 8: {
    style->is_underline = TRUE;
    style->text_alignment = e_FSAlignmentRight;
    break;
  }
  case 9:
    style->is_strikethrough = TRUE;
    break;
  case 10:
    style->mark_style = e_FSCornerMarkSubscript;
    break;
  case 11:
    style->mark_style = e_FSCornerMarkSuperscript;
    break;
  default:
    break;
  }
}

wchar_t* GetTableCellText(int index) {
  wchar_t* cell_text = L"";
  switch (index) {
  case 0:
    cell_text = L"Reference style";
    break;
  case 1:
    cell_text = L"Alignment center";
    break;
  case 2:
    cell_text = L"Green text color and alignment right";
    break;
  case 3:
    cell_text = L"Text font size 15";
    break;
  case 4:
    cell_text = L"Red text color, Times New Roman font and alignment right";
    break;
  case 5:
    cell_text = L"Bold, Times New Roman font and alignment right";
    break;
  case 6:
    cell_text = L"Bold and italic";
    break;
  case 7:
    cell_text = L"Bold, italic and alignment center";
    break;
  case 8:
    cell_text = L"Underline and alignment right";
    break;
  case 9:
    cell_text = L"Strikethrough";
    break;
  case 10:
    cell_text = L"CornerMarkSubscript";
    break;
  case 11:
    cell_text = L"CornerMarkSuperscript";
    break;
  default:
    cell_text = L" ";
    break;
  }
  return cell_text;
}

void AddElectronicTable(FS_PDFPAGE_HANDLE page) {
  // Add a spreadsheet with 4 rows and 3 columns
  {
    int index = 0;
    wchar_t* cell_text[12];
    FSTableCellData cell_array[4][3];
    int row, col;
    float page_width = 0;
    float page_height = 0;
    FSTableData data;
    FSRectF rect;
    FS_BOOL ret;
    FSErrorCode error_code;

    for (row = 0; row < 4; row++) {
      for (col = 0; col < 3; col++) {
        SetTableTextStyle(index, &cell_array[row][col].cell_text_style);
        cell_text[index] = GetTableCellText(index);
        cell_array[row][col].cell_text.str = (wchar_t*)cell_text[index];
        cell_array[row][col].cell_text.len = wcslen(cell_text[index]);
        cell_array[row][col].cell_image = 0;
        cell_array[row][col].cell_margin.left = 0;
        cell_array[row][col].cell_margin.right = 0;
        cell_array[row][col].cell_margin.bottom = 0;
        cell_array[row][col].cell_margin.top = 0;
        index++;
      }
    }

    FSDK_PDFPage_GetWidth(page, &page_width);
    FSDK_PDFPage_GetHeight(page, &page_height);

    data.row_count = 4;
    data.col_count = 3;
    data.outside_border_left.line_width = 1;
    data.outside_border_left.table_border_style = e_FSTableBorderStyleSolid;
    data.outside_border_left.color = 0xFF000000;
    data.outside_border_left.array_length_dashes = 0;
    data.outside_border_top.line_width = 1;
    data.outside_border_top.table_border_style = e_FSTableBorderStyleSolid;
    data.outside_border_top.color = 0xFF000000;
    data.outside_border_top.array_length_dashes = 0;
    data.outside_border_right.line_width = 1;
    data.outside_border_right.table_border_style = e_FSTableBorderStyleSolid;
    data.outside_border_right.color = 0xFF000000;
    data.outside_border_right.array_length_dashes = 0;
    data.outside_border_bottom.line_width = 1;
    data.outside_border_bottom.table_border_style = e_FSTableBorderStyleSolid;
    data.outside_border_bottom.color = 0xFF000000;
    data.outside_border_bottom.array_length_dashes = 0;
    data.inside_border_col.line_width = 1;
    data.inside_border_col.table_border_style = e_FSTableBorderStyleSolid;
    data.inside_border_col.color = 0xFF000000;
    data.inside_border_col.array_length_dashes = 0;
    data.inside_border_row.line_width = 1;
    data.inside_border_row.table_border_style = e_FSTableBorderStyleSolid;
    data.inside_border_row.color = 0xFF000000;
    data.inside_border_row.array_length_dashes = 0;
    data.merge_cells_length = 0;
    data.row_height_array = NULL;
    data.array_length_row_height = 0;
    data.col_width_array = NULL;
    data.array_length_col_width = 0;

    rect.left = 100;
    rect.right = page_width - 100;
    rect.top = page_height - 100;
    rect.bottom = 550;    
    data.rect = rect;

    error_code = FSDK_TableGenerator_AddTableToPage(page, data, (const FSTableCellData*)cell_array, 4, 3, &ret);
    if (error_code != e_FSErrSuccess) {
      printf("AddElectronicTable fail [%d]\r\n", error_code);
    }
  }

  // Add a spreadsheet with 5 rows and 6 columns
  {
    int index = 0;
    wchar_t* cell_text[5];
    FSTableCellData cell_array[5][6];
    int row, col;
    float page_width = 0;
    float page_height = 0;
    FSTableData data;
    FS_BOOL ret;
    FSErrorCode error_code;
    FSRectF rect;
    for (row = 0; row < 5; row++) {
      for (col = 0; col < 6; col++) {
        SetTableTextStyle(row, &cell_array[row][col].cell_text_style);
        switch (col) {
        case 0: {
          cell_array[row][col].cell_text.str = L"First column";
          cell_array[row][col].cell_text.len = wcslen(L"First column");
          cell_array[row][col].cell_image = 0;
          cell_array[row][col].cell_margin.left = 0;
          cell_array[row][col].cell_margin.right = 0;
          cell_array[row][col].cell_margin.bottom = 0;
          cell_array[row][col].cell_margin.top = 0;
          break;
        }
        case 1: {
          cell_array[row][col].cell_text.str = L"Second column";
          cell_array[row][col].cell_text.len = wcslen(L"Second column");
          cell_array[row][col].cell_image = 0;
          cell_array[row][col].cell_margin.left = 0;
          cell_array[row][col].cell_margin.right = 0;
          cell_array[row][col].cell_margin.bottom = 0;
          cell_array[row][col].cell_margin.top = 0;
          break;
        }
        case 2: {
          cell_array[row][col].cell_text.str = L"Third column";
          cell_array[row][col].cell_text.len = wcslen(L"Third column");
          cell_array[row][col].cell_image = 0;
          cell_array[row][col].cell_margin.left = 0;
          cell_array[row][col].cell_margin.right = 0;
          cell_array[row][col].cell_margin.bottom = 0;
          cell_array[row][col].cell_margin.top = 0;
          break;
        }
        case 3: {
          cell_array[row][col].cell_text.str = L"Fourth column";
          cell_array[row][col].cell_text.len = wcslen(L"Fourth column");
          cell_array[row][col].cell_image = 0;
          cell_array[row][col].cell_margin.left = 0;
          cell_array[row][col].cell_margin.right = 0;
          cell_array[row][col].cell_margin.bottom = 0;
          cell_array[row][col].cell_margin.top = 0;
          break;
        }
        case 4: {
          cell_array[row][col].cell_text.str = L"Fifth column";
          cell_array[row][col].cell_text.len = wcslen(L"Fifth column");
          cell_array[row][col].cell_image = 0;
          cell_array[row][col].cell_margin.left = 0;
          cell_array[row][col].cell_margin.right = 0;
          cell_array[row][col].cell_margin.bottom = 0;
          cell_array[row][col].cell_margin.top = 0;
          break;
        }
        case 5: {
          wchar_t return_string[MAX_FILE_PATH];
          FSDateTime date_time = GetLocalDateTime();
          swprintf_s(return_string, MAX_FILE_PATH, L"%d/%d/%d-%d:%d:%d %ls%d:%d", date_time.year, date_time.month, date_time.day, date_time.hour,
          date_time.minute, date_time.second, date_time.utc_hour_offset > 0 ? L"+" : L"-", date_time.utc_hour_offset,
          date_time.utc_minute_offset);
          cell_text[index] = return_string;
          FSDK_WStr_Init(&cell_array[row][col].cell_text);
          FSDK_WStr_SetLength(&cell_array[row][col].cell_text, wcslen(cell_text[index]));
          FSDK_WStr_Set(&cell_array[row][col].cell_text, cell_text[index], wcslen(cell_text[index]));
          cell_array[row][col].cell_image = 0;
          cell_array[row][col].cell_margin.left = 0;
          cell_array[row][col].cell_margin.right = 0;
          cell_array[row][col].cell_margin.bottom = 0;
          cell_array[row][col].cell_margin.top = 0;
          index++;
          break;
        }
        default:
          break;
        }
      }
    }

    FSDK_PDFPage_GetWidth(page, &page_width);
    FSDK_PDFPage_GetHeight(page, &page_height);

    data.row_count = 5;
    data.col_count = 6;
    data.outside_border_left.line_width = 2;
    data.outside_border_left.table_border_style = e_FSTableBorderStyleSolid;
    data.outside_border_left.color = 0xFF000000;
    data.outside_border_left.array_length_dashes = 0;
    data.outside_border_top.line_width = 2;
    data.outside_border_top.table_border_style = e_FSTableBorderStyleSolid;
    data.outside_border_top.color = 0xFF000000;
    data.outside_border_top.array_length_dashes = 0;
    data.outside_border_right.line_width = 2;
    data.outside_border_right.table_border_style = e_FSTableBorderStyleSolid;
    data.outside_border_right.color = 0xFF000000;
    data.outside_border_right.array_length_dashes = 0;
    data.outside_border_bottom.line_width = 2;
    data.outside_border_bottom.table_border_style = e_FSTableBorderStyleSolid;
    data.outside_border_bottom.color = 0xFF000000;
    data.outside_border_bottom.array_length_dashes = 0;
    data.inside_border_col.line_width = 2;
    data.inside_border_col.table_border_style = e_FSTableBorderStyleSolid;
    data.inside_border_col.color = 0xFF000000;
    data.inside_border_col.array_length_dashes = 0;
    data.inside_border_row.line_width = 2;
    data.inside_border_row.table_border_style = e_FSTableBorderStyleSolid;
    data.inside_border_row.color = 0xFF000000;
    data.inside_border_row.array_length_dashes = 0;
    data.merge_cells_length = 0;
    data.row_height_array = NULL;
    data.array_length_row_height = 0;
    data.col_width_array = NULL;
    data.array_length_col_width = 0;

    rect.left = 10;
    rect.right = page_width - 10;
    rect.top = page_height - 350;
    rect.bottom = 200;
    data.rect = rect;

    error_code = FSDK_TableGenerator_AddTableToPage(page, data, (const FSTableCellData*)cell_array, 5, 6, &ret);
    if (error_code != e_FSErrSuccess) {
      printf("AddElectronicTable fail [%d]\r\n", error_code);
    }
  }
}


int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t output_directory[MAX_FILE_PATH];
  FSErrorCode error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lselectronictable/", output_path);
  _wmkdir(output_directory);

  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    wchar_t output_file[MAX_FILE_PATH];
	FS_BOOL is_save;
    FSDK_PDFDoc_Create(&doc);

    FSDK_PDFDoc_InsertPage0(doc, 0, e_FSSizeLetter, &page);
    AddElectronicTable(page);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lselectronictable_result.pdf", output_directory);
 
    FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &is_save);
    FSDK_PDFPage_Release(page);
    FSDK_PDFDoc_Release(doc);
    printf("electronictable demo.\n");
  }  
  FSDK_Library_Release();
  return err_ret;
}