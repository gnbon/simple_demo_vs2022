// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to export data, import data and reset form for
// XFA document.

// Include Foxit SDK header files.
#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_pdfform_c.h"
#include "../../../include/fs_xfa_c.h"

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

static void gRelease(void* user_data) {
  if (user_data) free(user_data);
}

static FS_WSTR gGetAppInfo(void* user_data,FSAppInfo app_info) {
  FS_WSTR wstr;
  wstr.str = L"Foxit SDK";
  wstr.len = wcslen(L"Foxit SDK");
  return wstr;
}

static void gBeep(void* user_data, FSBeepType type) {}

static FSMsgBoxButtonID gMsgBox(void* user_data, const wchar_t* message, const wchar_t* title, FSMsgBoxIconType icon_type,
  FSMsgBoxButtonType button_type) {
  return (FSMsgBoxButtonID)0;
}

static FS_WSTR gResponse(void* user_data, const wchar_t* question, const wchar_t* title, const wchar_t* default_answer,
    FS_BOOL is_mask) {
  FS_WSTR wstr;
  wstr.str = L"answer";
  wstr.len = wcslen(L"answer");
  return wstr;
}
  
static FSReaderCallback* gDownLoadUrl(void* user_data, const wchar_t* url) {
  return NULL;
}

static FS_WSTR gPostRequestURL(void* user_data, const wchar_t* url, const wchar_t* data, const wchar_t* content_type, const wchar_t* encode, const wchar_t* header) {
  FS_WSTR wstr;
  wstr.str = L"PostRequestUrl";
  wstr.len = wcslen(L"PostRequestUrl");
  return wstr;
}

static FS_BOOL gPutRequestURL(void* user_data, const wchar_t* url, const wchar_t* data, const wchar_t* encode) {
  return TRUE;
}

static FS_WSTR gLoadString(void* user_data, FSStringID string_id) {
  FS_WSTR wstr;
  wstr.str = L"LoadString";
  wstr.len = wcslen(L"LoadString");
  return wstr;
}

static void gShowFileDialog(void* user_data, const wchar_t* string_title, const wchar_t* string_filter, FS_BOOL is_openfile_dialog, FS_WSTR* return_stringarray, FS_UINT32* return_stringarray_length) {}

static void gXFADocProviderRelease(void* user_data) {
  if (user_data) free(user_data);
}

static void gInvalidateRect(void* user_data, int page_index, const FSRectF rect, FSInvalidateFlag flag) {
}

static void gDisplayCaret(void* user_data, int page_index, FS_BOOL is_visible, const FSRectF rect) {}

static FS_BOOL gGetPopupPos(void* user_data, int page_index, float min_popup, float max_popup, const FSRectF rect_widget, FSRectF rect_popup) {
  return 1;
}

static FS_BOOL gPopupMenu(void* user_data, int page_index, const FSPointF rect_popup) {
  return 1;
}

static int gGetCurrentPage(void* user_data, const FS_XFADOC_HANDLE doc) {
  return 0;
}

static void gSetCurrentPage(void* user_data, const FS_XFADOC_HANDLE doc, int current_page_index) {}

static FS_WSTR gGetTitle(void* user_data, const FS_XFADOC_HANDLE doc) {
  FS_WSTR title;
  title.str = L"title";
  title.len = wcslen(L"title");
  return title;
}

static void gExportData(void* user_data, const FS_XFADOC_HANDLE doc, const wchar_t* file_path) {}
static void gImportData(void* user_data, const FS_XFADOC_HANDLE doc, const wchar_t* file_path) {}
static void gGotoURL(void* user_data, const FS_XFADOC_HANDLE doc, const wchar_t* url) {}
static void gPrint(void* user_data, const FS_XFADOC_HANDLE doc, int start_page_index, int end_page_index, FS_UINT32 options) {}
static FSDK_ARGB gGetHighlightColor(void* user_data, const FS_XFADOC_HANDLE doc) {
  FSXFAType xfatype;
  FS_XFADOC_HANDLE doc_new = doc;
  FSDK_XFADoc_GetType(doc_new, &xfatype);
  if (xfatype == e_FSStatic) {
    return 0x50FF0000;
  } else {
    return 0x500000FF;
  }
}

static FS_BOOL gSubmitData(void* user_data, const FS_XFADOC_HANDLE doc, const wchar_t* target, FSSubmitFormat format, FSTextEncoding text_encoding, const wchar_t* content) {
  return 1;
}

static void gSetFocus(void* user_data, FS_XFAWIDGET_HANDLE xfa_widget) {};

static void gPageViewEvent(void* user_data, int page_index, FSPageViewEventType page_view_event_type) {}

static void gSetChangeMark(void* user_data, const FS_XFADOC_HANDLE doc) {}

static void gWidgetEvent(void* user_data, const FS_XFAWIDGET_HANDLE xfa_widget, FSWidgetEventType widget_event_type) {}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  FSAppProviderCallback* xfa_app_provider = NULL;
  FS_PDFDOC_HANDLE doc = NULL;
  FSDocProviderCallback* xfa_doc_handler = NULL;
  FS_XFADOC_HANDLE xfa_doc = NULL;
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  wchar_t output_file_new[MAX_FILE_PATH] = { 0 };
  FS_BOOL return_exportdata ;
  wchar_t output_file_new2[MAX_FILE_PATH] = { 0 };
  FS_BOOL return_saveas;  
  FS_BOOL return_importdata;
  wchar_t output_file_new3[MAX_FILE_PATH] = { 0 };
  FS_BOOL return_saveas2;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsxfa_dynamic.pdf", input_path);
  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsxfa_form/", output_path);
  _wmkdir(output_directory);

  xfa_app_provider = (FSAppProviderCallback*)malloc(sizeof(FSAppProviderCallback));
  xfa_app_provider->user_data = xfa_app_provider;
  xfa_app_provider->Release = gRelease;
  xfa_app_provider->GetAppInfo = gGetAppInfo;
  xfa_app_provider->Beep = gBeep;
  xfa_app_provider->MsgBox = gMsgBox;
  xfa_app_provider->FSResponse = gResponse;
  xfa_app_provider->DownLoadUrl = gDownLoadUrl;
  xfa_app_provider->PostRequestURL = gPostRequestURL;
  xfa_app_provider->PutRequestURL = gPutRequestURL;
  xfa_app_provider->LoadString = gLoadString;
  xfa_app_provider->ShowFileDialog = gShowFileDialog;

  FSDK_Library_RegisterXFAAppProviderCallback(xfa_app_provider);

  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }
  
  xfa_doc_handler = (FSDocProviderCallback*)malloc(sizeof(FSDocProviderCallback));
  xfa_doc_handler->user_data = xfa_doc_handler;
  xfa_doc_handler->Release = gXFADocProviderRelease;
  xfa_doc_handler->InvalidateRect = gInvalidateRect;
  xfa_doc_handler->DisplayCaret = gDisplayCaret;
  xfa_doc_handler->GetPopupPos = gGetPopupPos;
  xfa_doc_handler->PopupMenu = gPopupMenu;
  xfa_doc_handler->GetCurrentPage = gGetCurrentPage;
  xfa_doc_handler->SetCurrentPage = gSetCurrentPage;
  xfa_doc_handler->SetChangeMark = gSetChangeMark;
  xfa_doc_handler->GetTitle = gGetTitle;
  xfa_doc_handler->ExportData = gExportData;
  xfa_doc_handler->ImportData = gImportData;
  xfa_doc_handler->GotoURL = gGotoURL;
  xfa_doc_handler->Print = gPrint;
  xfa_doc_handler->GetHighlightColor = gGetHighlightColor;
  xfa_doc_handler->SubmitData = gSubmitData;
  xfa_doc_handler->PageViewEvent = gPageViewEvent;
  xfa_doc_handler->WidgetEvent = gWidgetEvent;

  FSDK_XFADoc_Create(doc, xfa_doc_handler, &xfa_doc);

  FSDK_XFADoc_StartLoad(xfa_doc, NULL, &progressive);

  swprintf_s(output_file_new, MAX_FILE_PATH, L"%lsxfa_form.xml", output_directory);

  FSDK_XFADoc_ExportData(xfa_doc, output_file_new, e_FSExportDataTypeXML, &return_exportdata);

  FSDK_XFADoc_ResetForm(xfa_doc);

  swprintf_s(output_file_new2, MAX_FILE_PATH, L"%lsxfa_dynamic_resetform.pdf", output_directory);

  FSDK_PDFDoc_SaveAs(doc, output_file_new2, e_FSSaveFlagsSaveFlagNormal, &return_saveas);

  FSDK_XFADoc_ImportData(xfa_doc, output_file_new, &return_importdata);

  swprintf_s(output_file_new3, MAX_FILE_PATH, L"%lsxfa_dynamic_importdata.pdf", output_directory);

  FSDK_PDFDoc_SaveAs(doc, output_file_new3, e_FSSaveFlagsSaveFlagNormal, &return_saveas2);

  FSDK_XFADoc_Release(xfa_doc);
  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();

  printf("Xfa test.\n");
  return err_ret;
}
