// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to do PDF page organization,
// such as inserting, removing, and so on.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_pdfgraphicsobject_c.h"

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

wchar_t output_directory[MAX_FILE_PATH] = { 0 };

void AddTextObjects(FS_PDFPAGE_HANDLE page, const wchar_t* content) {
  FS_POSITION position;
  FS_TEXTOBJECT_HANDLE text_object = NULL;
  FSTextState state;
  FS_POSITION position2;
  FSRectF rect;
  float page_width, page_height, offset_x, offset_y;
  FSMatrix matrix;
  FS_BOOL return_transform;
  FS_BOOL return_generatecontent;
  FS_FONT_HANDLE font = NULL;

  FSDK_GraphicsObjects_GetLastGraphicsObjectPosition(page, e_FSTypeText, &position);
  FSDK_TextObject_Create(&text_object);

  FSDK_GraphicsObject_SetFillColor(text_object, 0xFFAAAAAA);
  FSDK_GraphicsObject_SetStrokeColor(text_object, 0xFFF68C21);

  // Prepare text state
  state.font_size = 64.0f;

  FSDK_Font_Create0(e_FSStdIDTimes, &font);
  state.font = font;
  state.textmode = e_FSModeFillStrokeClip;
  state.charspace = 0;
  state.origin_position.x = 0;
  state.origin_position.y = 0;
  state.wordspace = 0;
  state.version = 1;
  state.textmatrix[0] = 1;
  state.textmatrix[1] = 0;
  state.textmatrix[2] = 0;
  state.textmatrix[3] = 1;
  FSDK_TextObject_SetTextState(text_object, page, state, FALSE, 750);

  // Set content
  FSDK_TextObject_SetText(text_object, content);
  FSDK_GraphicsObjects_InsertGraphicsObject(page, position, text_object, &position2);
  // Transform to center
  FSDK_GraphicsObject_GetRect(text_object, &rect);

  FSDK_PDFPage_GetWidth(page, &page_width);
  FSDK_PDFPage_GetHeight(page, &page_height);
  offset_x = (page_width - (rect.right - rect.left)) / 2;
  offset_y = (page_height - (rect.top - rect.bottom)) / 2;

  matrix.a = 1;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = 1;
  matrix.e = offset_x;
  matrix.f = offset_y;

  FSDK_GraphicsObject_Transform(text_object, matrix, FALSE, &return_transform);

  // Generator content
  FSDK_GraphicsObjects_GenerateContent(page, &return_generatecontent);
}

FS_BOOL InsertPage() {
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;
  FSErrorCode code = e_FSErrSuccess;
  FS_PDFPAGE_HANDLE page = NULL;
  int icount;
  wchar_t output_file[MAX_FILE_PATH] = { 0 };
  FS_BOOL return_result = FALSE;

  swprintf_s(input_file, MAX_FILE_PATH, L"%lspage_organization_123.pdf", input_path);
  code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return 1;
  }

  FSDK_PDFDoc_InsertPage0(doc, -1, e_FSSizeLetter, &page);
  AddTextObjects(page, L"insert in first");
  FSDK_PDFPage_Release(page);

  FSDK_PDFDoc_InsertPage0(doc, 2, e_FSSizeLetter, &page);
  AddTextObjects(page, L"insert in 2 (based 0)");
  FSDK_PDFPage_Release(page);

  FSDK_PDFDoc_GetPageCount(doc, &icount);
  FSDK_PDFDoc_InsertPage0(doc, icount, e_FSSizeLetter, &page);
  AddTextObjects(page, L"insert in last");
  FSDK_PDFPage_Release(page);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsinsert_page.pdf", output_directory);

  code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }

  FSDK_PDFDoc_Release(doc);
  printf("Insert pages.\n");
  return TRUE;
}

FS_BOOL RemovePage() {
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;
  FSErrorCode code = e_FSErrSuccess;
  int icount;
  wchar_t output_file[MAX_FILE_PATH] = { 0 };
  FS_BOOL return_result = FALSE;

  swprintf_s(input_file, MAX_FILE_PATH, L"%lspage_organization_123.pdf", input_path);

  code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return FALSE;
  }

  FSDK_PDFDoc_GetPageCount(doc, &icount);

  while (icount > 1) {
    FS_BOOL return_value;
    FSDK_PDFDoc_RemovePage(doc, 0, &return_value);
    FSDK_PDFDoc_GetPageCount(doc, &icount);
  }

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsremove_all_pages_exceptt_the_last_page.pdf", output_directory);

  code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return FALSE;
  }
  FSDK_PDFDoc_Release(doc);
  printf("Remove pages.\n");
  return TRUE;
}

// You can implement the functionality of exporting pages by creating an empty document
// and importing part of the source document into the empty document.
// You can also import the target document as a source document
// to implement the copy page functionality.
FS_BOOL ImportPage() {
  wchar_t file_dest[MAX_FILE_PATH] = { 0 };
  wchar_t file_src[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc_dest = NULL;
  FSErrorCode code = e_FSErrSuccess;
  FS_PDFDOC_HANDLE doc_src = NULL;
  FS_RANGE_HANDLE import_ranges = NULL;
  int icount;
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  wchar_t output_file[MAX_FILE_PATH] = { 0 };
  FS_BOOL return_result = FALSE;

  swprintf_s(file_dest, MAX_FILE_PATH, L"%lspage_organization_123.pdf", input_path);
  swprintf_s(file_src, MAX_FILE_PATH, L"%lspage_organization_abc.pdf", input_path);

  code = FSDK_PDFDoc_Create0(file_dest, &doc_dest);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc_dest, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_dest);
    wprintf(L"The Doc [%ls] Error: %d\n", file_dest, code);
    return 1;
  }

  code = FSDK_PDFDoc_Create0(file_src, &doc_src);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc_src, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_src);
    wprintf(L"The Doc [%ls] Error: %d\n", file_src, code);
    return 1;
  }

  // Import page from PDFDoc object.
  FSDK_Range_Create0(0, &import_ranges);
  FSDK_PDFDoc_GetPageCount(doc_src, &icount);
  FSDK_Range_AddSingle(import_ranges, icount - 1);
  FSDK_PDFDoc_StartImportPages(doc_dest, -1, doc_src, e_FSImportPageFlagsImportFlagNormal, "abc", import_ranges, NULL, &progressive);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lspage_organization_abc_to_123_import_pages.pdf", output_directory);

  code = FSDK_PDFDoc_SaveAs(doc_dest, output_file, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc_dest);
    return 1;
  }

  FSDK_PDFDoc_Release(doc_src);
  FSDK_PDFDoc_Release(doc_dest);
  code = FSDK_PDFDoc_Create0(file_dest, &doc_dest);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc_dest, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_dest);
    wprintf(L"The Doc [%ls] Error: %d\n", file_dest, code);
    return FALSE;
  }
  // Import page from file path.
  FSDK_PDFDoc_StartImportPagesFromFilePath(doc_dest, -1, file_src, NULL, e_FSImportPageFlagsImportFlagNormal, "abc", import_ranges, NULL, &progressive);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsabc_to_123_import_pages_form_filepath.pdf", output_directory);
  code = FSDK_PDFDoc_SaveAs(doc_dest, output_file, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc_dest);
    return FALSE;
  }
  FSDK_PDFDoc_Release(doc_dest);
  FSDK_Range_Release(import_ranges);
  printf("Import pages.\n");
  return TRUE;
}

// This example shows how to use the use the functions StartExtractPages and InsertDocument together,
// instead of the StartImportPagesFromFilePath function. The execution efficiency of importing pages has
// been optimized in these new functions.
FS_BOOL ImportPageOptimized() {
  wchar_t file_dest[MAX_FILE_PATH] = { 0 };
  wchar_t file_src[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc_dest = NULL;
  FSErrorCode code = e_FSErrSuccess;
  FS_RANGE_HANDLE import_ranges = NULL;
  int icount;
  wchar_t file_temp[MAX_FILE_PATH] = { 0 };
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  FS_PDFDOC_HANDLE doc_temp = NULL;
  wchar_t output_file[MAX_FILE_PATH] = { 0 };
  FS_BOOL return_result = FALSE;
  FS_PDFDOC_HANDLE doc_src = NULL;

  swprintf_s(file_dest, MAX_FILE_PATH, L"%lspage_organization_123.pdf", input_path);
  swprintf_s(file_src, MAX_FILE_PATH, L"%lspage_organization_abc.pdf", input_path);

  code = FSDK_PDFDoc_Create0(file_dest, &doc_dest);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc_dest, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_dest);
    wprintf(L"The Doc [%ls] Error: %d\n", file_dest, code);
    return FALSE;
  }

  code = FSDK_PDFDoc_Create0(file_src, &doc_src);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc_src, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_src);
    wprintf(L"The Doc [%ls] Error: %d\n", file_src, code);
    return FALSE;
  }
  // Extract pages to temporary document
  FSDK_Range_Create0(0, &import_ranges);
  FSDK_PDFDoc_GetPageCount(doc_src, &icount);
  FSDK_Range_AddSingle(import_ranges, icount - 1);

  swprintf_s(file_temp, MAX_FILE_PATH, L"%lspage_organization_import_extracted_pages.pdf", output_directory);

  FSDK_PDFDoc_StartExtractPages(doc_src, file_temp, e_FSExtractPagesOptionAnnotation, import_ranges, NULL, &progressive);

  code = FSDK_PDFDoc_Create0(file_temp, &doc_temp);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc_temp, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_temp);
    wprintf(L"The Doc [%ls] Error: %d\n", file_temp, code);
    return FALSE;
  }

  // Insert temporary document into destination
  FSDK_PDFDoc_InsertDocument(doc_dest, -1, doc_temp, e_FSInsertDocOptionAttachments);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsabc_to_123_insert_document.pdf", output_directory);

  code = FSDK_PDFDoc_SaveAs(doc_dest, output_file, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc_dest);
    return FALSE;
  }

  FSDK_Range_Release(import_ranges);
  FSDK_PDFDoc_Release(doc_src);
  FSDK_PDFDoc_Release(doc_temp);
  FSDK_PDFDoc_Release(doc_dest);

  printf("Import pages optimized.\n");
  return TRUE;
}

FS_BOOL MovePages() {
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;
  wchar_t input_file1[MAX_FILE_PATH] = { 0 };
  FSErrorCode code = e_FSErrSuccess;
  int count = 0 , i = 0;
  FS_PDFPAGE_HANDLE page = NULL;
  FS_BOOL return_result = FALSE;
  wchar_t output_file1[MAX_FILE_PATH] = { 0 };
  FS_RANGE_HANDLE page_set = NULL;
  wchar_t output_file[MAX_FILE_PATH] = { 0 };

  swprintf_s(input_file, MAX_FILE_PATH, L"%lspage_organization_123.pdf", input_path);
  swprintf_s(input_file1, MAX_FILE_PATH, L"%lspage_organization_abc.pdf", input_path);

  // Move page
  code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return FALSE;
  }

  FSDK_PDFDoc_GetPageCount(doc, &count);

  code = FSDK_PDFDoc_GetPage(doc, 0, &page);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    return FALSE;
  }

  FSDK_PDFDoc_MovePageTo(doc, page, count - 1, &return_result);

  swprintf_s(output_file1, MAX_FILE_PATH, L"%lsmove_first_page_to_last.pdf", output_directory);

  code = FSDK_PDFDoc_SaveAs(doc, output_file1, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return FALSE;
  }
  FSDK_PDFPage_Release(page);
  FSDK_PDFDoc_Release(doc);

  code = FSDK_PDFDoc_Create0(input_file1, &doc);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, code);
    return FALSE;
  }

  FSDK_Range_Create0(0, &page_set);
  for (i = 0; i < count / 2; i++)
    FSDK_Range_AddSingle(page_set, 2 * i);
  FSDK_PDFDoc_GetPageCount(doc, &count);
  FSDK_PDFDoc_MovePagesTo(doc, page_set, count - 1, &return_result);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsmove_pages.pdf", output_directory);

  code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return FALSE;
  }

  FSDK_Range_Release(page_set);
  FSDK_PDFDoc_Release(doc);
  printf("Move pages.\n");
  return TRUE;
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }
  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspage_organization/", output_path);
  _wmkdir(output_directory);

  if (!(InsertPage() && RemovePage() && ImportPage() && ImportPageOptimized() && MovePages())) {
    return 1;
  }

  FSDK_Library_Release();
  printf("page organization demo done.\n");
  return err_ret;
}
