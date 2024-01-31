// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to do full text search in PDF documents under a folder.

// Include Foxit SDK header files.
#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_fulltextsearch_c.h"

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

static const char* output_path = "../output_files/";
static const char* input_path = "../input_files/fulltextsearch/";

void WriteUnicode(FILE* file, const wchar_t* text_content) {
  FS_WSTR str;
  FS_BSTR dst_bstr;

  str.str = (wchar_t*)text_content;
  str.len = wcslen(text_content);
  FSDK_WStr_UTF16LEEncode(&str, &dst_bstr);

  if (dst_bstr.len == 0) return;
  fwrite((const char*)dst_bstr.str, sizeof(char), dst_bstr.len, file);
}

FILE* file = NULL;

void gRelease(void* user_data) { 
  free(user_data); 
}

int gRetrieveSearchResult(void* user_data, const wchar_t* file_path, int page_index, const wchar_t* match_result, int match_start_text_index, int match_end_text_index, const wchar_t* sentence, int sentence_start_text_index, int sentence_end_text_index) {
  wchar_t result_str[1024] = { 0 };
  swprintf_s(result_str, 1024, L"RetrieveSearchResult:\nFound file is: %ls\nPage index is: %d\nStart text index: %d\nEnd text index: %d\nMatch is: %ls\nSentence start index:%d\nnSentence end index:%d\nSentence is:%ls\n", file_path, page_index, match_start_text_index, match_end_text_index, (const wchar_t*)match_result, sentence_start_text_index, sentence_end_text_index, (const wchar_t*)sentence);
  WriteUnicode(file, result_str);
  WriteUnicode(file, L"\r\n");

  return 0;
}

int pause_count_limit_ = 5;
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
  FSErrorCode error_code = e_FSErrSuccess;
  char output_directory[MAX_FILE_PATH];
  char output_file[MAX_FILE_PATH];
  char dbpath[MAX_FILE_PATH];

  sprintf_s(output_directory, MAX_FILE_PATH, "%sfulltextsearch/", output_path);
  _mkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  {
    FS_FULLTEXTSEARCH_HANDLE full_text_search = NULL;
    FS_DOCUMENTSSOURCE_HANDLE docs_source = NULL;
    FS_BOOL return_value;
    FS_PROGRESSIVE_HANDLE search_progress;
    FSPauseCallback* pausecallback = NULL;
    FSState state = e_FSToBeContinued;
    FSSearchCallback* search_callback = NULL;

    FSDK_FullTextSearch_Create(&full_text_search);
    //The path of data base to store the indexed data...
    sprintf_s(dbpath, MAX_FILE_PATH, "%ssearch.db", output_directory);
    FSDK_FullTextSearch_SetDataBasePath(full_text_search, dbpath);
    // Get document source information.

    FSDK_DocumentsSource_Create(input_path, &docs_source);
    // Create a Pause callback object implemented by users to pause the updating process.
    pausecallback = (FSPauseCallback*)malloc(sizeof(FSPauseCallback));
    pausecallback->user_data = pausecallback;
    pausecallback->NeedToPauseNow = gNeedToPauseNow;

    FSDK_FullTextSearch_StartUpdateIndex(full_text_search, docs_source, pausecallback, FALSE, &search_progress);

    while (state == e_FSToBeContinued) {
      FSDK_Progressive_Continue(search_progress, &state);
    }
    // Create a callback object which will be invoked when a matched one is found.
    search_callback = (FSSearchCallback*)malloc(sizeof(FSSearchCallback));
    search_callback->Release = gRelease;
    search_callback->user_data = search_callback;
    search_callback->RetrieveSearchResult = gRetrieveSearchResult;
    sprintf_s(output_file, MAX_FILE_PATH, "%sresult.txt", output_directory);
    fopen_s(&file, output_file, "w+b");
    // Search the specified keyword from the indexed data source.
    FSDK_FullTextSearch_SearchOf(full_text_search, "Foxit", e_FSRankHitCountASC, search_callback, &return_value);
    FSDK_FullTextSearch_Release(full_text_search);
    fclose(file);
    free(pausecallback);
    FSDK_Library_Release();
    printf("FullTextSearch demo.\n");
  }
  return 0;
}