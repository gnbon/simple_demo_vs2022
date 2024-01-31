// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to redact some content in a PDF document.

#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_search_c.h"
#include "../../../include/fs_redaction_c.h"
#include "../../../include/fs_render_c.h"

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
  FS_PDFDOC_HANDLE doc = NULL;
  FS_REDACTION_HANDLE redaction = NULL;
  FS_PDFPAGE_HANDLE page = NULL;
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  FS_TEXTPAGE_HANDLE text_page = NULL;
  FS_TEXTSEARCH_HANDLE text_search = NULL;
  FS_BOOL return_code;
  FSRectF* rectarray = NULL;
  int count = 0;
  FS_BOOL return_findtext;
  int i = 0;
  wchar_t save_pdf_path3[MAX_FILE_PATH] = { 0 };
  FS_PROGRESSIVE_HANDLE return_startsaveas;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdfredact/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  FSDK_Redaction_Create(doc, &redaction);
  // Parse PDF page.
  FSDK_PDFDoc_GetPage(doc, 0, &page);
  FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);
  FSDK_TextPage_Create(page, 0, &text_page);
  FSDK_TextSearch_Create1(text_page, &text_search);
  FSDK_TextSearch_SetPattern(text_search, L"PDF", &return_code);

  FSDK_TextSearch_FindNext(text_search, &return_findtext);

  while (return_findtext) {
    FSRectF* rect = NULL;
    FS_UINT32 return_array_length = 0;
    FS_UINT32 i = 0;
    FSDK_TextSearch_GetMatchRects(text_search, rect, &return_array_length);
    rect = (FSRectF*)malloc(sizeof(FSRectF) * return_array_length);
    FSDK_TextSearch_GetMatchRects(text_search, rect, &return_array_length);
    for (i = 0; i < return_array_length; i++) {
      count++;
      rectarray = (FSRectF*)realloc(rectarray, count * sizeof(FSRectF));
      rectarray[count - 1] = rect[i];
    }
    free(rect);
    FSDK_TextSearch_FindNext(text_search, &return_findtext);
  }

  if (count > 0) {
    FS_REDACTION_HANDLE redact = NULL;
    FS_BOOL return_code;
    wchar_t save_pdf_path[MAX_FILE_PATH] = { 0 };
    wchar_t save_pdf_path1[MAX_FILE_PATH] = { 0 };
    FS_PROGRESSIVE_HANDLE return_startsaveas2;
    wchar_t save_pdf_path2[MAX_FILE_PATH] = { 0 };
    FS_PROGRESSIVE_HANDLE return_startsaveas3;
    FS_PROGRESSIVE_HANDLE return_startsavess = NULL;

    FSDK_Redaction_MarkRedactAnnot(redaction, page, rectarray, count, &redact);

    FSDK_Annot_ResetAppearanceStream(redact, &return_code);
    swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%lsAboutFoxit_redected_default.pdf", output_directory);

    FSDK_PDFDoc_StartSaveAs(doc, save_pdf_path, e_FSSaveFlagNoOriginal, NULL, &return_startsavess);

    // Set border color to green.
    FSDK_Annot_SetBorderColor(redact, (long)0x00FF00);
    // Set fill color to blue.
    FSDK_Redact_SetFillColor(redact, (long)0x0000FF);
    // Set rollover fill color to red.
    FSDK_Redact_SetApplyFillColor(redact, (long)0xFF0000);
    FSDK_Annot_ResetAppearanceStream(redact, &return_code);

    swprintf_s(save_pdf_path1, MAX_FILE_PATH, L"%lsAboutFoxit_redected_setColor.pdf", output_directory);

    FSDK_PDFDoc_StartSaveAs(doc, save_pdf_path1, e_FSSaveFlagNoOriginal, NULL, &return_startsaveas2);

    FSDK_Markup_SetOpacity(redact, (float)0.5);
    FSDK_Annot_ResetAppearanceStream(redact, &return_code);

    swprintf_s(save_pdf_path2, MAX_FILE_PATH, L"%lsAboutFoxit_redected_setOpacity.pdf", output_directory);

    FSDK_PDFDoc_StartSaveAs(doc, save_pdf_path2, e_FSSaveFlagNoOriginal, NULL, &return_startsaveas3);

    if (FSDK_Redaction_Apply(redaction, &return_code) == e_FSErrSuccess)
      printf("Redact page(0) succeed.\n");
    else
      printf("Redact page(0) failed.\n");

    FSDK_Redact_Release(redact);
  }

  swprintf_s(save_pdf_path3, MAX_FILE_PATH, L"%lsAboutFoxit_redected_apply.pdf", output_directory);

  FSDK_PDFDoc_StartSaveAs(doc, save_pdf_path3, e_FSSaveFlagNoOriginal, NULL, &return_startsaveas);

  FSDK_Redaction_Release(redaction);
  FSDK_TextPage_Release(text_page);
  FSDK_TextSearch_Release(text_search);
  FSDK_PDFPage_Release(page);
  FSDK_PDFDoc_Release(doc);

  FSDK_Library_Release();
  printf("Redact test.\n");
  return err_ret;
}

