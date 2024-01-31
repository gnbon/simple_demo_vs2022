// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to search text in all pages in PDF document.

// Include Foxit SDK header files.
#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdarg.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_search_c.h"

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
#define MAX_BUFFER_PATH 1024

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

void WriteUnicode(FILE* file, const wchar_t* text_content) {
  FS_WSTR str;
  FS_BSTR dst_bstr;

  str.str = (wchar_t*)text_content;
  str.len = wcslen(text_content);
  FSDK_WStr_UTF16LEEncode(&str, &dst_bstr);

  if (dst_bstr.len == 0) return;
  fwrite((const char*)dst_bstr.str, sizeof(char), dst_bstr.len, file);
}

void OutputMatchedInfo(FILE* file, FS_TEXTSEARCH_HANDLE search, int matched_index) {
  int page_index = 0, stratcharindex  = 0, endcharindex, startindex;
  FS_WSTR sentence;
  FSRectF* rect_array = NULL;
  FS_UINT32 return_array_length;
  int rect_count = 0, i = 0;
  wchar_t filebuffer[MAX_BUFFER_PATH];
  memset(filebuffer, 0, MAX_BUFFER_PATH);

  FSDK_TextSearch_GetMatchPageIndex(search, &page_index);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"Index of matched pattern:\t%d\r\n", matched_index);
  WriteUnicode(file, filebuffer);

  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"\tpage:\t%d\r\n", page_index);
  WriteUnicode(file, filebuffer);

  FSDK_TextSearch_GetMatchStartCharIndex(search, &stratcharindex);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"\tmatch char start index:\t%d\r\n", stratcharindex);
  WriteUnicode(file, filebuffer);

  FSDK_TextSearch_GetMatchEndCharIndex(search, &endcharindex);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"\tmatch char end index:\t%d\r\n", endcharindex);
  WriteUnicode(file, filebuffer);

  FSDK_TextSearch_GetMatchSentenceStartIndex(search, &startindex);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"\tmatch sentence start index:\t%d\r\n", startindex);
  WriteUnicode(file, filebuffer);

  FSDK_WStr_Init(&sentence);
  FSDK_TextSearch_GetMatchSentence(search, &sentence);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"\tmatch sentence:\t%ls\r\n", sentence.str);
  WriteUnicode(file, filebuffer);

  FSDK_TextSearch_GetMatchRects(search, NULL, &return_array_length);
  rect_array = (FSRectF*)malloc(return_array_length * sizeof(FSRectF));
  FSDK_TextSearch_GetMatchRects(search, rect_array, &return_array_length);

  rect_count = return_array_length;
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"\tmatch rectangles count:\t%d\r\n", rect_count);
  WriteUnicode(file, filebuffer);
  for (i = 0; i < rect_count; i++) {
    FSRectF rect = rect_array[i];
    swprintf_s(filebuffer, MAX_BUFFER_PATH, L"\trectangle(in PDF space) :%d\t[left = %.4f, bottom = %.4f, right = %.4f, top = %.4f]\r\n", i,
      rect.left, rect.bottom, rect.right, rect.top);
    WriteUnicode(file, filebuffer);
  }
}

int main(int argc, char *argv[]) {
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t input_file[MAX_FILE_PATH];
  wchar_t output_file[MAX_FILE_PATH];
  int err_ret = 0;
  FS_PDFDOC_HANDLE doc = NULL;
  FILE* file = NULL;
  FS_TEXTSEARCH_HANDLE search;
  int page_count;
  int start_index = 0, end_index = 0; 
  FS_BOOL return_value;
  FS_BOOL return_value2;
  const wchar_t* pattern = L"Foxit";
  FS_BOOL return_value3;
  FS_UINT32 flags = e_FSSearchFlagsSearchNormal;
  FS_BOOL return_value4;
  wchar_t* match_case;
  wchar_t* match_whole_word;
  wchar_t* match_consecutive;
  int match_count = 0;
  FS_BOOL return_value5;
  unsigned char temp[] = { 0xFF,0xFE };
  wchar_t filebuffer[MAX_BUFFER_PATH];

  memset(filebuffer, 0, MAX_BUFFER_PATH);
  swprintf_s(output_directory, MAX_FILE_PATH, L"%lssearch/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lssearch.txt", output_directory);
  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  _wfopen_s(&file, output_file, L"w+b");
  fwrite(temp, sizeof(unsigned char), 2, file);
  fseek(file, 0, SEEK_END);

  // sample 1: search for all pages of doc.
  FSDK_TextSearch_Create(doc, NULL, e_FSTextParseFlagsParseTextNormal, &search);

  FSDK_PDFDoc_GetPageCount(doc, &page_count);
  end_index = page_count - 1;
  FSDK_TextSearch_SetStartPage(search, 0, &return_value);

  FSDK_TextSearch_SetEndPage(search, page_count - 1, &return_value2);

  FSDK_TextSearch_SetPattern(search, pattern, &return_value3);
  // If want to specify flags, you can do as followings:
  // flags |= TextSearch::e_SearchMatchCase;
  // flags |= TextSearch::e_SearchMatchWholeWord;
  // flags |= TextSearch::e_SearchConsecutive;
  FSDK_TextSearch_SetSearchFlags(search, flags, &return_value4);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"Begin search %ls at %ls.\n", pattern, input_file);
  WriteUnicode(file, filebuffer);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"Start index:\t%d\r\n", start_index);
  WriteUnicode(file, filebuffer);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"End index:\t%d\r\n", end_index);
  WriteUnicode(file, filebuffer);
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"Match key:\t%ls\r\n", pattern);
  WriteUnicode(file, filebuffer);
  match_case = flags & e_FSSearchMatchCase ? L"Yes" : L"No";
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"Match Case\t%ls\r\n", (const wchar_t*)match_case);
  WriteUnicode(file, filebuffer);
  match_whole_word = flags & e_FSSearchMatchWholeWord ? L"Yes" : L"No";
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"Match whole word:\t%ls\r\n", (const wchar_t*)match_whole_word);
  WriteUnicode(file, filebuffer);
  match_consecutive = flags & e_FSSearchConsecutive ? L"Yes" : L"No";
  swprintf_s(filebuffer, MAX_BUFFER_PATH, L"Consecutive:\t%ls\r\n", (const wchar_t*)match_consecutive);
  WriteUnicode(file, filebuffer);

  FSDK_TextSearch_FindNext(search, &return_value5);
  while (return_value5) {
    OutputMatchedInfo(file, search, match_count);
    match_count++;
    FSDK_TextSearch_FindNext(search, &return_value5);
  }
  fclose(file);
  printf("Marched %d counts\n", match_count);
  FSDK_Library_Release();
  printf("Search demo finished.\n");

  return err_ret;
}

