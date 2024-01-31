// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to do optimize in PDF documents under a folder.

// Include Foxit SDK header files.
#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_optimization_c.h"

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
static const wchar_t* input_path = L"../input_files/optimization/";

int pause_count_limit_ = 0;
int pause_count_ = 0;
FS_BOOL always_pause_ = TRUE;

FS_BOOL gNeedToPauseNow(void* user_data) {
  if (always_pause_) return TRUE;
  if (pause_count_ < pause_count_limit_) {
    pause_count_++;
    return TRUE;
  }
  if (pause_count_ < pause_count_limit_) {
    pause_count_++;
    return TRUE;
  } else {
     pause_count_ = 0;
     return FALSE; // This is to test a case: valid PauseCallback but needParseNow() will always return FALSE.
  }
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t input_file[MAX_FILE_PATH];

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsoptimization/", output_path);
  _wmkdir(output_directory);

  printf("Optimized Start : Image Compression.\n");
  swprintf_s(input_file, MAX_FILE_PATH, L"%ls[Optimize]ImageCompression.pdf", input_path);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FSPauseCallback* pause = NULL;
    FS_OPTIMIZERSETTINGS_HANDLE settings;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FSState progress_state = e_FSToBeContinued;
    int percent = 0;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    pause = (FSPauseCallback*)malloc(sizeof(FSPauseCallback));
    pause->user_data = pause;
    pause->NeedToPauseNow = gNeedToPauseNow;

    FSDK_OptimizerSettings_Create(&settings);
    error_code = FSDK_OptimizerSettings_SetOptimizerOptions(settings, e_FSOptimizerCompressImages);

    FSDK_Optimizer_Optimize(doc, settings, pause, &progressive);

    while (e_FSToBeContinued == progress_state) {
      char res_string[60];
      FSDK_Progressive_Continue(progressive, &progress_state);
      FSDK_Progressive_GetRateOfProgress(progressive, &percent);
      sprintf_s(res_string, 60, "Optimize progress percent: %d", percent);
      printf("%s\n", res_string);
    }
    if (e_FSFinished == progress_state) {
      wchar_t output_directory_optimized[60];
      FS_BOOL return_result = FALSE;
      swprintf_s(output_directory_optimized, 60, L"%lsImageCompression_Optimized.pdf", output_directory);
      
      error_code = FSDK_PDFDoc_SaveAs(doc, output_directory_optimized, e_FSSaveFlagRemoveRedundantObjects, &return_result);
      if (error_code != e_FSErrSuccess) {
        printf("Save doc error!\n");
        FSDK_OptimizerSettings_Release(settings);
        FSDK_PDFDoc_Release(doc);
        return 1;
      }
    }
    FSDK_OptimizerSettings_Release(settings);
  }
  printf("Optimized Finsih : Image Compression.\n");

  printf("Optimized Start : Clean up.\n");

  swprintf_s(input_file, MAX_FILE_PATH, L"%ls[Optimize]Cleanup.pdf", input_path);
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FSPauseCallback* pause = NULL;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FSState progress_state = e_FSToBeContinued;
    FS_OPTIMIZERSETTINGS_HANDLE settings;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    pause_count_limit_ = 0;
    pause_count_ = 0;
    always_pause_ = TRUE;
    pause = (FSPauseCallback*)malloc(sizeof(FSPauseCallback));
    pause->user_data = pause;
    pause->NeedToPauseNow = gNeedToPauseNow;

    FSDK_OptimizerSettings_Create(&settings);
    FSDK_OptimizerSettings_SetOptimizerOptions(settings, e_FSOptimizerCleanUp);
    FSDK_OptimizerSettings_SetCleanUpOptions(settings, e_FSCleanUpRemoveInvalidBookmarks | e_FSCleanUpRemoveInvalidLinks
      | e_FSCleanUpUseFlateForNonEncodedStream | e_FSCleanUpUseFlateInsteadOfLZW);

    FSDK_Optimizer_Optimize(doc, settings, pause, &progressive);

    while (e_FSToBeContinued == progress_state) {
      int percent;
      char res_string[60];
      FSDK_Progressive_Continue(progressive, &progress_state);
      FSDK_Progressive_GetRateOfProgress(progressive, &percent);
      sprintf_s(res_string, 60, "Optimize progress percent: %d", percent);
      printf("%s\n", res_string);
    }
    if (e_FSFinished == progress_state) {
      wchar_t output_directory_optimized[MAX_FILE_PATH];
      FS_BOOL return_result = FALSE;
      swprintf_s(output_directory_optimized, MAX_FILE_PATH, L"%lsCleanup_Optimized.pdf", output_directory);
      error_code = FSDK_PDFDoc_SaveAs(doc, output_directory_optimized, e_FSSaveFlagRemoveRedundantObjects, &return_result);
      if (error_code != e_FSErrSuccess) {
        printf("Save doc error!\n");
        FSDK_OptimizerSettings_Release(settings);
        FSDK_PDFDoc_Release(doc);
        return 1;
      }
    }
    FSDK_OptimizerSettings_Release(settings);
  }
  printf("Optimized Finsih : Clean up.\n");

  printf("Optimized Start : Discard Objects.\n");

  swprintf_s(input_file, MAX_FILE_PATH, L"%ls[Optimize]DiscardObjects.pdf", input_path);
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FSPauseCallback* pause = NULL;
    FS_OPTIMIZERSETTINGS_HANDLE settings;
	FS_PROGRESSIVE_HANDLE progressive = NULL;
	FSState progress_state;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    pause_count_limit_ = 0;
    pause_count_ = 0;
    always_pause_ = TRUE;
    pause = (FSPauseCallback*)malloc(sizeof(FSPauseCallback));
    pause->user_data = pause;
    pause->NeedToPauseNow = gNeedToPauseNow;

    FSDK_OptimizerSettings_Create(&settings);
    FSDK_OptimizerSettings_SetOptimizerOptions(settings, e_FSOptimizerDiscardObjects);
    FSDK_OptimizerSettings_SetCleanUpOptions(settings, e_FSDiscardObjectsBookmarks | e_FSDiscardObjectsEmbeddedPageThumbnails
      | e_FSDiscardObjectsEmbeddedPrintSettings | e_FSDiscardObjectsFlattenFormFields
      | e_FSDiscardObjectsFormActions | e_FSDiscardObjectsJavaScriptActions);

    FSDK_Optimizer_Optimize(doc, settings, pause, &progressive);
    progress_state = e_FSToBeContinued;
    while (e_FSToBeContinued == progress_state) {
      int percent;
      char res_string[60];
      FSDK_Progressive_Continue(progressive, &progress_state);
      FSDK_Progressive_GetRateOfProgress(progressive, &percent);
      sprintf_s(res_string, 60, "Optimize progress percent: %d", percent);
      printf("%s\n", res_string);
    }
    if (e_FSFinished == progress_state) {
      wchar_t output_directory_optimized[MAX_FILE_PATH];
      FS_BOOL return_result = FALSE;
      swprintf_s(output_directory_optimized, MAX_FILE_PATH, L"%lsDiscardObjects_Optimized.pdf", output_directory);

      error_code = FSDK_PDFDoc_SaveAs(doc, output_directory_optimized, e_FSSaveFlagRemoveRedundantObjects, &return_result);
      if (error_code != e_FSErrSuccess) {
        printf("Save doc error!\n");
        FSDK_OptimizerSettings_Release(settings);
        FSDK_PDFDoc_Release(doc);
        return 1;
      }
    }
    FSDK_OptimizerSettings_Release(settings);
  }
  printf("Optimized Finsih : Discard Objects.\n");
  FSDK_Library_Release();
  return err_ret;
}