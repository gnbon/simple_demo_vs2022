// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to convert Word or Excel files to PDF files.

// Include Foxit SDK header files.
#if defined(_WIN32) || defined(_WIN64)

#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_convert_c.h"

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
  wchar_t output_directory[MAX_FILE_PATH];
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_file[MAX_FILE_PATH];
  wchar_t word_file_path[MAX_FILE_PATH];
  wchar_t excel_file_path[MAX_FILE_PATH];
  wchar_t ppt_file_path[MAX_FILE_PATH];

  // Use default Word2PDFSettingData values.
  FSWord2PDFSettingData word_convert_setting_data;
  // Use default Excel2PDFSettingData values.
  FSExcel2PDFSettingData excel_convert_setting_data;
  // Use default PowerPoint2PDFSettingData values.
  FSPowerPoint2PDFSettingData ppt_convert_setting_data;

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsoffice2pdf/", output_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  // Conver Word file to PDF file.
  swprintf_s(word_file_path, MAX_FILE_PATH, L"%lsAboutFoxit.doc", input_path);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lsword2pdf_result.pdf", output_directory);

  //set default values
  word_convert_setting_data.include_doc_props = FALSE;
  word_convert_setting_data.optimize_option = e_FSConvertOptimizeOptionForPrint;
  word_convert_setting_data.content_option = e_FSConvertContentOptionOnlyContent;
  word_convert_setting_data.bookmark_option = e_FSConvertBookmarkOptionNone;
  word_convert_setting_data.convert_to_pdfa = FALSE;

  error_code = FSDK_Convert_FromWord(word_file_path, L"", output_file, word_convert_setting_data);
  if (error_code != e_FSErrSuccess) {
    printf("Convert Word file to PDF file failed..Error_Code: %d\n", error_code);
  } else {
    printf("Convert Word file to PDF file.\n");
  }

  // Conver Excel file to PDF file.
  swprintf_s(excel_file_path, MAX_FILE_PATH, L"%lstest.xlsx", input_path);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lsexcel2pdf_result.pdf", output_directory);

  //set default values
  excel_convert_setting_data.include_doc_props = FALSE;
  excel_convert_setting_data.quality = e_FSConvertQualityStandard;
  excel_convert_setting_data.ignore_print_area = TRUE;
  excel_convert_setting_data.scale_type = e_FSScaleTypeNone;
  excel_convert_setting_data.convert_to_pdfa = FALSE;

  error_code = FSDK_Convert_FromExcel(excel_file_path, L"", output_file, excel_convert_setting_data);
  if (error_code != e_FSErrSuccess) {
    printf("Convert Excel file to PDF file failed.Error_Code:%d\n", error_code);
  } else {
    printf("Convert Excel file to PDF file.\n");
  }

  // Conver PowerPoint file to PDF file.
  swprintf_s(ppt_file_path, MAX_FILE_PATH, L"%lstest.ppt", input_path);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lsppt2pdf_result.pdf", output_directory);

  //set default values
  ppt_convert_setting_data.intent = e_FSConvertIntentPrint;
  ppt_convert_setting_data.frame_output_slides = FALSE;
  ppt_convert_setting_data.output_type = e_FSOutputSlides;
  ppt_convert_setting_data.handout_order = e_FSHandoutOrderVerticalFirst;
  ppt_convert_setting_data.output_hidden_slides = FALSE;
  ppt_convert_setting_data.include_doc_props = FALSE;

  error_code = FSDK_Convert_FromPowerPoint(ppt_file_path, L"", output_file, ppt_convert_setting_data);
  if (error_code != e_FSErrSuccess) {
    printf("Convert PowerPoint file to PDF file failed.Error_Code:%d\n", error_code);
  }
  else {
    printf("Convert PowerPoint file to PDF file.\n");
  }
  
  return 0;
}
#endif  // #if defined(_WIN32) || defined(_WIN64)

