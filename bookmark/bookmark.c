// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to enumerate and modify bookmarks
// in PDF document.

// Include Foxit SDK header files.
#include <time.h>
#include <stdarg.h>
#include <direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_filespec_c.h"
#include "../../../include/fs_bookmark_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_action_c.h"
#include "../../../include/fx_stream_c.h"

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

void Write(FILE* file, const wchar_t* format, ...) {
  va_list vars;
  if (file) {
    // Get variable list
    va_start(vars, format);
    // Call vfprintf to format log data and output to log file
    vfwprintf(file, format, vars);
    // End variable list
    va_end(vars);
  }
}

void ShowBookmarkInfo(FS_BOOKMARK_HANDLE bookmark, int depth, FILE* file) {
  FS_BOOL return_value;
  FS_WSTR title;
  FS_RGB rgb;
  FS_BOOKMARK_HANDLE firstchild = NULL;
  FS_BOOKMARK_HANDLE nextsibling = NULL;
  int i = 0;

  if (depth > 32)
    return;

  FSDK_Bookmark_IsEmpty(bookmark, &return_value);
  if (return_value)
    return;
  for (i = 0; i < depth; i++) {
    Write(file, L"\t");
  }

  // Show bookmark's title.
  FSDK_WStr_Init(&title);
  FSDK_Bookmark_GetTitle(bookmark, &title);
  Write(file, L"%s\t", title.str);

  // Show bookmark's color.
  FSDK_Bookmark_GetColor(bookmark, &rgb);
  Write(file, L"color: %x\r\n", rgb);

  FSDK_Bookmark_GetFirstChild(bookmark, &firstchild);
  FSDK_Bookmark_GetNextSibling(bookmark, &nextsibling);
  ShowBookmarkInfo(firstchild, depth + 1,file);
  ShowBookmarkInfo(nextsibling, depth,file);
}

void ShowBookmarksInfo(FS_PDFDOC_HANDLE doc, const wchar_t* info_path, FILE* file) {
  FS_BOOKMARK_HANDLE root = NULL;
  FS_BOOL return_value;
  FSDK_PDFDoc_GetRootBookmark(doc, &root);
  FSDK_Bookmark_IsEmpty(root, &return_value);
  if (!return_value) {
    ShowBookmarkInfo(root, 0, file);
  } else {
    Write(file, L"No bookmark information!\r\n");
  }
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t output_directory[MAX_FILE_PATH];

  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  wchar_t output_file1[MAX_FILE_PATH];
  wchar_t output_file2[MAX_FILE_PATH];
  wchar_t bookmark_info_file[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE doc = NULL;
  FILE* file = NULL;
  FS_BOOKMARK_HANDLE root = NULL;
  FS_BOOL return_value;
  int count = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  int i = 0;
  FS_BOOL is_save;
  FS_BOOKMARK_HANDLE first_bookmark;
  FS_BOOKMARK_HANDLE next_sibling;
  FS_BOOL is_empty;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsbookmark/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
  swprintf_s(output_file1, MAX_FILE_PATH, L"%lsbookmark_add.pdf", output_directory);

  swprintf_s(output_file2, MAX_FILE_PATH, L"%lsbookmark_change.pdf", output_directory);

  swprintf_s(bookmark_info_file, MAX_FILE_PATH, L"%lsbookmark_info.txt", output_directory);

  FSDK_PDFDoc_Create0(input_file, &doc);
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  _wfopen_s(&file, bookmark_info_file, L"w+b");

  // Show original bookmark information.
  ShowBookmarksInfo(doc, bookmark_info_file, file);

  //Get bookmark root node or Create new bookmark root node.
  FSDK_PDFDoc_GetRootBookmark(doc, &root);
  FSDK_Bookmark_IsEmpty(root, &return_value);
  if (return_value) {
    FSDK_PDFDoc_CreateRootBookmark(doc, root);
  }

  FSDK_PDFDoc_GetPageCount(doc, &count);
  for (i = 0; i < count; i += 2) {
    FS_DESTINATION_HANDLE dest = NULL;
    wchar_t ws_title[100];
    FS_BOOKMARK_HANDLE child = NULL;

    FSDK_Destination_CreateFitPage(doc, i, &dest);

    swprintf_s(ws_title, 100, L"A bookmark to a page (index: %d)", i);

    FSDK_Bookmark_Insert(root, ws_title, e_FSPosLastChild, &child);
    FSDK_Bookmark_SetDestination(child, dest);
    FSDK_Bookmark_SetColor(child, i * 0xF68C21);
  }

  error_code = FSDK_PDFDoc_SaveAs(doc, output_file1, e_FSSaveFlagNoOriginal, &is_save);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }

  fclose(file);

  // Get first bookmark and change properties.
  FSDK_Bookmark_GetFirstChild(root, &first_bookmark);
  FSDK_Bookmark_SetStyle(first_bookmark, e_FSStyleItalic);
  FSDK_Bookmark_SetColor(first_bookmark, 0xFF0000);
  FSDK_Bookmark_SetTitle(first_bookmark, L"Change bookmark title, style, and color");

  // Remove next sibling bookmark
  FSDK_Bookmark_GetNextSibling(first_bookmark, &next_sibling);
  FSDK_Bookmark_IsEmpty(next_sibling, &is_empty);
  if (!is_empty) {
    FS_BOOL is_remove;
    FSDK_PDFDoc_RemoveBookmark(doc, next_sibling, &is_remove);
  }

  swprintf_s(bookmark_info_file, MAX_FILE_PATH, L"%lsbookmark_info1.txt", output_directory);
  _wfopen_s(&file, bookmark_info_file, L"w+b");
  ShowBookmarksInfo(doc, bookmark_info_file, file);

  error_code = FSDK_PDFDoc_SaveAs(doc, output_file2, e_FSSaveFlagNoOriginal, &is_save);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }

  fclose(file);
  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();
  printf("Bookmark demo.\n");

  return err_ret;
}

