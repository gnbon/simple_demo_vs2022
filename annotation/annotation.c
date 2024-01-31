// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to add various annotations to PDF document.

#include <locale.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include Foxit SDK header files.
#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_action_c.h"
#include "../../../include/fs_signature_c.h"
#include "../../../include/fs_actioncallback_c.h"
#include "../../../include/fs_annot_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfobject_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_psi_c.h"
#include "../../../include/fs_filespec_c.h"
#include "../../../include/fs_image_c.h"
#include "../../../include/fs_rendition_c.h"

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


typedef struct _hash_node {
  char *key;
  FS_PDFDOC_HANDLE value;
  struct _hash_node *next;
} hash_node;

typedef struct _hash_map {
  int size;
  hash_node *nodes;
} hash_map;

hash_map *hash_map_create() {
  hash_map *h_map = (hash_map*)malloc(sizeof(hash_map));
  if (h_map == NULL) return NULL;
  h_map->size = 0;
  h_map->nodes = NULL;
  return h_map;
}

void hash_map_destroy(hash_map* h_map) {
  hash_node *node = h_map->nodes;
  while (node) {
    hash_node *next = node->next;
    free(node->key);
    free(node);
    node = next;
  }
  free(h_map);
}

void hash_map_put(hash_map* h_map, const char* key, FS_PDFDOC_HANDLE value) {
  hash_node *node = h_map->nodes;
  hash_node *new_node = NULL;
  while (node) {
    if (strcmp(node->key, key) == 0) {
      node->value = value;
      return;
    }
    node = node->next;
  }
  new_node = (hash_node *)malloc(sizeof(hash_node));
  if (!new_node) {
    return;
  }
  new_node->key = calloc(strlen(key) + 1, sizeof(char));
  memcpy(new_node->key, key, strlen(key) + 1);
  new_node->value = value;
  new_node->next = h_map->nodes;
  h_map->nodes = new_node;
  h_map->size++;
}

FS_PDFDOC_HANDLE hash_map_get(hash_map* h_map, const char *key) {
  hash_node *node = h_map->nodes;
  while (node) {
    if (strcmp(node->key, key) == 0) {
      return node->value;
    }
    node = node->next;
  }
  return NULL;
}

FSDateTime GetLocalDateTime() {
  time_t t = time(NULL);
  struct tm _Tm;
  struct tm* rime;
  long time_zone = 0;
  int timezone_hour = 0;
  int timezone_minute = 0;
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

int string2wstring(const char *source, size_t source_size, wchar_t *dest, size_t dest_size) {
  const char *_source;
  wchar_t *_dest;
  setlocale(LC_ALL, "chs");
  if (!source || !dest) return 0;
  _source = source;
  _dest = dest;
  wmemset(_dest, 0, dest_size);
  mbstowcs(_dest, _source, dest_size);
  setlocale(LC_ALL, "C");
  return (int)dest_size;
}

char* RandomUID(char *uid, int uid_size) {
  const char* c = "0123456789qwertyuiopasdfghjklzxcvbnm";
  char *p = NULL;
  int n = 0;
  if (!uid || 37 != uid_size) return 0;
  p = uid;
  memset(p, 0, uid_size);  
  for (n = 0; n < 16; n++) {
    int b = rand() % 255;
    switch (n) {
    case 6:
      sprintf_s(p, 3,  "4%x", b % 15);
      break;
    case 8:
      sprintf_s(p, 3, "%c%x", c[rand() % strlen(c)], b % 15);
      break;
    default:
      sprintf_s(p, 3, "%02x", b);
      break;
    }
    p = p + 2;
    if (3 == n || 5 == n || 7 == n || 9 == n) {
      *p = '-';
      p++;
    }
  }
  return uid;
}

wchar_t* WRandomUID(wchar_t *uid, int uid_size) {
  char c_uid[37];
  if (!uid) return L"";
  RandomUID(c_uid, sizeof(c_uid));
  string2wstring(c_uid, sizeof(c_uid), uid, uid_size);
  return uid;
}

void FreeFWSTR(FS_WSTR* wstr) {
  if(!wstr && !wstr->str)
    FSDK_WStr_Clear(wstr);
}

void InitFWSTR(FS_WSTR* wstr, const wchar_t* text) {
  FreeFWSTR(wstr);
  FSDK_WStr_Init(wstr);
  FSDK_WStr_SetLength(wstr, (FS_UINT32)wcslen(text) + 1);
  FSDK_WStr_Set(wstr, text, (FS_UINT32)wcslen(text)+1);
}

void FreeFSBTR(FS_BSTR* wstr) {
  if (!wstr && !wstr->str)
    FSDK_BStr_Clear(wstr);
}

void InitFSBTR(FS_BSTR* wstr, const char* text) {
  FreeFSBTR(wstr);
  FSDK_BStr_Init(wstr);
  FSDK_BStr_SetLength(wstr, (FS_UINT32)strlen(text) + 1);
  FSDK_BStr_Set(wstr, text, (FS_UINT32)strlen(text) + 1);
}

typedef struct _IconProviderData {
  hash_map *pdf_doc_map_;
  FS_BOOL use_dynamic_stamp_;
  char file_folder_[MAX_FILE_PATH];
  FS_BSTR provider_id;
  FS_BSTR provider_version;
}IconProviderData;

IconProviderData gicon_provider_data;

void gRelease(void* user_data) {
  FreeFSBTR(&gicon_provider_data.provider_id);
  FreeFSBTR(&gicon_provider_data.provider_version);
  hash_map_destroy(gicon_provider_data.pdf_doc_map_);
  if (user_data)  free(user_data);
}

// If one icon provider offers different style icon for one icon name of a kind of annotaion,
// please use different provider ID or version in order to distinguish different style for Foxit PDF SDK.
// Otherwise, only the first style icon for the same icon name of same kind of annotation will have effect.
FS_BSTR gGetProviderID(void* user_data) {
  if (gicon_provider_data.use_dynamic_stamp_)
    InitFSBTR(&gicon_provider_data.provider_id, "Simple Demo Dynamic IconProvider");
  else
    InitFSBTR(&gicon_provider_data.provider_id, "Simple Demo IconProvider");
  return gicon_provider_data.provider_id;
}

FS_BSTR  gGetProviderVersion(void* user_data) { 
  InitFSBTR(&gicon_provider_data.provider_version, "1.0.0");  
  return gicon_provider_data.provider_version;
}

FS_BOOL gHasIcon(void* user_data, FSAnnotType annot_type, const char* icon_name) {
  char path[MAX_FILE_PATH];
  int pagecount = -1;
  FS_PDFDOC_HANDLE doc = NULL;
  hash_node* node = NULL;
  if (gicon_provider_data.use_dynamic_stamp_) {
    sprintf_s(path, MAX_FILE_PATH, "%s/DynamicStamps/%s.pdf", gicon_provider_data.file_folder_, icon_name);
  }
  else {
    sprintf_s(path, MAX_FILE_PATH, "%s/StaticStamps/%s.pdf", gicon_provider_data.file_folder_, icon_name);
  }

  node = (hash_node*)hash_map_get(gicon_provider_data.pdf_doc_map_, path);
  if (node) {
    FS_PDFDOC_HANDLE doc = node->value;
    return !(doc == (void*)0);
  }

  FSDK_PDFDoc_Create(&doc);

  FSDK_PDFDoc_GetPageCount(doc, &pagecount);
  if (pagecount == 0) {
    wchar_t wpath[MAX_FILE_PATH];
    FSErrorCode error_code;
    string2wstring(path, MAX_FILE_PATH, wpath, MAX_FILE_PATH);
    FSDK_PDFDoc_Release(doc);
    doc = NULL;
    FSDK_PDFDoc_Create0(wpath, &doc);
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (e_FSErrSuccess != error_code) {
      FSDK_PDFDoc_Release(doc);
      doc = NULL;
    } else {
      hash_map_put(gicon_provider_data.pdf_doc_map_, path, doc);
    }
  }
  return doc != NULL;
}

FS_BOOL gCanChangeColor(void* user_data, FSAnnotType annot_type, const char* icon_name) {
  return FALSE;
}

FS_PDFPAGE_HANDLE gGetIcon(void* user_data, FSAnnotType annot_type, const char* icon_name, FSDK_ARGB color) {
  FS_PDFPAGE_HANDLE page_handle = NULL;
  char path[MAX_FILE_PATH] = { 0 };
  int page_count = 0;
  FS_PDFDOC_HANDLE doc;
  FS_PDFDOC_HANDLE hand;
  if (gicon_provider_data.use_dynamic_stamp_) {
    sprintf_s(path, MAX_FILE_PATH, "%s/DynamicStamps/%s.pdf", gicon_provider_data.file_folder_, icon_name);
  }
  else {
    sprintf_s(path, MAX_FILE_PATH, "%s/StaticStamps/%s.pdf", gicon_provider_data.file_folder_, icon_name);
  }
  doc = (hash_node*)hash_map_get(gicon_provider_data.pdf_doc_map_, path);

  hand = (void*)0;  
  if (!doc)
    return page_handle;
  FSDK_PDFDoc_GetPageCount(doc, &page_count);
  if (page_count == 0) return page_handle;
  FSDK_PDFDoc_GetPage(doc, 0, &page_handle);
  return page_handle;
}

FS_BOOL gGetShadingColor(void* user_data, FSAnnotType annot_type, const char* icon_name,
  FS_RGB referenced_color, int shading_index, FSShadingColor out_shading_color) {
  return FALSE;
}

float gGetDisplayWidth(void* user_data, FSAnnotType annot_type, const char* icon_name) {
  return 0.0f;
}

float gGetDisplayHeight(void* user_data, FSAnnotType annot_type, const char* icon_name) {
  return 0.0f;
}


typedef struct _ActionCallbackData{
  FS_PDFDOC_HANDLE current_doc_;
  FS_WSTR response_;
  FS_WSTR file_path_;
  FS_WSTR attachments_file_path_;
  FS_WSTR extracted_embedded_file_path_;
  FS_WSTR browse_file_;
  FS_WSTR browse_file0_;
  FSIdentityProperties identity_properties_;
  FS_WSTR popup_menu_;
  FSMenuItemEx menu_ex;
  FS_WSTR app_info;
  FS_WSTR untitled_bookmark_name_;
  FS_WSTR maildoc_;
  FS_WSTR temporary_file_name_;
  FS_WSTR temporary_directory_;
  FSSearchIndexConfig search_index_config_;
}ActionCallbackData;

ActionCallbackData gaction_callback_data;

void gActionRelease(void* user_data) {
  FreeFWSTR(&gaction_callback_data.response_);
  FreeFWSTR(&gaction_callback_data.file_path_);
  FreeFWSTR(&gaction_callback_data.attachments_file_path_);
  FreeFWSTR(&gaction_callback_data.extracted_embedded_file_path_);
  FreeFWSTR(&gaction_callback_data.browse_file_);
  FreeFWSTR(&gaction_callback_data.browse_file0_);
  FreeFWSTR(&gaction_callback_data.popup_menu_);
  FreeFWSTR(&gaction_callback_data.untitled_bookmark_name_);
  FreeFWSTR(&gaction_callback_data.maildoc_);
  FreeFWSTR(&gaction_callback_data.temporary_file_name_);
  FreeFWSTR(&gaction_callback_data.temporary_directory_);

  FreeFWSTR(&gaction_callback_data.identity_properties_.corporation);
  FreeFWSTR(&gaction_callback_data.identity_properties_.email);
  FreeFWSTR(&gaction_callback_data.identity_properties_.login_name);
  FreeFWSTR(&gaction_callback_data.identity_properties_.name);
  FreeFWSTR(&gaction_callback_data.identity_properties_.first_name);
  FreeFWSTR(&gaction_callback_data.identity_properties_.last_name);
  FreeFWSTR(&gaction_callback_data.identity_properties_.title);
  FreeFWSTR(&gaction_callback_data.identity_properties_.department);

  if (user_data)  free(user_data);  
}

FS_BOOL gInvalidateRect(void* user_data, const FS_PDFDOC_HANDLE document, int page_index, const FSRectF pdf_rect) {
  return 0;
}

int gGetCurrentPage(void* user_data, const FS_PDFDOC_HANDLE* document) {
  return -1;
}

void gSetCurrentPage(void* user_data, const FS_PDFDOC_HANDLE* document, int page_index) {}

void gSetCurrentPage0(void* user_data, const FS_PDFDOC_HANDLE* document, const FS_DESTINATION_HANDLE* destination) {}

FSRotation gGetPageRotation(void* user_data, const FS_PDFDOC_HANDLE* document, int page_index) {
  return e_FSRotation0;
}

FS_BOOL gSetPageRotation(void* user_data, const FS_PDFDOC_HANDLE* document, int page_index, FSRotation rotation) {
  return FALSE;
}

FS_BOOL gExecuteNamedAction(void* user_data, const FS_PDFDOC_HANDLE* document, const char* named_action) {
  return FALSE;
}

FS_BOOL gSetDocChangeMark(void* user_data, const FS_PDFDOC_HANDLE* document, FS_BOOL change_mark) {
  return FALSE;
}

FS_BOOL gGetDocChangeMark(void* user_data, const FS_PDFDOC_HANDLE* document) {
  return FALSE;
}

int gGetOpenedDocCount(void* user_data) {
  return -1;
}

FS_PDFDOC_HANDLE gGetOpenedDoc(void* user_data, int index) {  
  return gaction_callback_data.current_doc_;
}

FS_PDFDOC_HANDLE gGetCurrentDoc(void* user_data) {  
  return gaction_callback_data.current_doc_;
}

FS_PDFDOC_HANDLE gCreateBlankDoc(void* user_data) {
  FS_PDFDOC_HANDLE tmpdoc = (FS_PDFDOC_HANDLE)NULL;
  return tmpdoc;
}

FS_PDFDOC_HANDLE gOpenDoc(void* user_data, const wchar_t* file_path, const FS_WSTR* password) {
  FS_PDFDOC_HANDLE tmpdoc = (FS_PDFDOC_HANDLE)NULL;
  return tmpdoc;
}

void gCloseDoc(void* user_data, const FS_PDFDOC_HANDLE* document, FS_BOOL is_prompt_to_save) {}

FS_BOOL gBeep(void* user_data, int type) {
  return FALSE;
}

FS_WSTR gResponse(void* user_data, const wchar_t* question, const wchar_t* title, const wchar_t* default_value, const wchar_t* label,
  FS_BOOL is_password) {  
  FSDK_WStr_Init(&gaction_callback_data.response_);
  return gaction_callback_data.response_;
}

FS_WSTR gGetFilePath(void* user_data, const FS_PDFDOC_HANDLE* document) {  
  FSDK_WStr_Init(&gaction_callback_data.file_path_);
  return gaction_callback_data.file_path_;
}

FS_BOOL gIsLocalFile(void *user_data, const FS_PDFDOC_HANDLE* document) {
  return FALSE;
}

FS_WSTR gGetAttachmentsFilePath(void* user_data, const FS_PDFDOC_HANDLE* pdf_doc, const wchar_t* name) {  
  FSDK_WStr_Init(&gaction_callback_data.attachments_file_path_);
  return gaction_callback_data.attachments_file_path_;
}

FS_WSTR gGetExtractedEmbeddedFilePath(void* user_data, const FS_PDFDOC_HANDLE* pdf_doc, const wchar_t* name) {  
  FSDK_WStr_Init(&gaction_callback_data.extracted_embedded_file_path_);
  return gaction_callback_data.extracted_embedded_file_path_;
}

FS_BOOL gPrint(void* user_data, const FS_PDFDOC_HANDLE* document, FS_BOOL is_ui,
  const FS_RANGE_HANDLE* page_set, FS_BOOL is_silent,
  FS_BOOL is_shrunk_to_fit, FS_BOOL is_printed_as_image,
  FS_BOOL is_reversed, FS_BOOL is_to_print_annots) {
  return FALSE;
}

FS_BOOL gPrint0(void* user_data, const FS_PDFDOC_HANDLE document, const FSPrintParams print_params) {
  return FALSE;
}

FS_BOOL gSubmitForm(void* user_data, const FS_PDFDOC_HANDLE* document, void* form_data, FS_UINT32 length, const char* url, FSFileFormatType file_format_type) {
  return FALSE;
}

FS_BOOL gLaunchURL(void* user_data, const char* url) {
  return FALSE;
}

FS_WSTR gBrowseFile(void* user_data) {  
  FSDK_WStr_Init(&gaction_callback_data.browse_file_);
  return gaction_callback_data.browse_file_;
}

FS_WSTR gBrowseFile0(void* user_data, FS_BOOL is_open_dialog, const wchar_t* file_format, const wchar_t* file_filter) {  
  FSDK_WStr_Init(&gaction_callback_data.browse_file0_);
  return gaction_callback_data.browse_file0_;
}

FSLanguage gGetLanguage(void* user_data) {
  return e_FSLanguageCHS;
}

int gAlert(void* user_data, const wchar_t* msg, const wchar_t* title, int type, int icon) {
  return 0;
}

FSIdentityProperties gGetIdentityProperties(void* user_data) {
  InitFWSTR(&gaction_callback_data.identity_properties_.corporation, L"foxitsoftware");
  InitFWSTR(&gaction_callback_data.identity_properties_.email, L"simple_demo@foxitsoftware.cn");
  InitFWSTR(&gaction_callback_data.identity_properties_.login_name, L"simple demo");
  InitFWSTR(&gaction_callback_data.identity_properties_.name, L"simple");
  InitFWSTR(&gaction_callback_data.identity_properties_.first_name, L"simple");
  InitFWSTR(&gaction_callback_data.identity_properties_.last_name, L"demo");
  InitFWSTR(&gaction_callback_data.identity_properties_.title, L"developer");
  InitFWSTR(&gaction_callback_data.identity_properties_.department, L"gsdk");

  return gaction_callback_data.identity_properties_;
}

FS_BOOL gSetIdentityProperties(void* user_data, const FSIdentityProperties identity_properties) {
  return FALSE;
}

FS_WSTR gPopupMenu(void* user_data, FSMenuList* menulist_array, FS_UINT32 array_length_menus, FS_BOOL is_selected_item) {
  FSDK_WStr_Init(&gaction_callback_data.popup_menu_);  
  return gaction_callback_data.popup_menu_;
}

FSMenuItemEx gPopupMenuEx(void* user_data, FSMenuItemEx* menuitem_array, FS_UINT32 array_length_menus, FS_BOOL is_selected_item) {
  FSMenuItemEx menu_ex;
  FSDK_WStr_Init(&menu_ex.item_name);
  FSDK_WStr_Init(&menu_ex.return_name);
  menu_ex.sub_menu_item_array = NULL;
  menu_ex.sub_menu_item_array_length = 0;
  return menu_ex;
}

FS_WSTR gGetAppInfo(void* user_data, FSAppInfoType type) {  
  FSDK_WStr_Init(&gaction_callback_data.app_info);
  return gaction_callback_data.app_info;
}

FS_BOOL gMailData(void* user_data, void* data, FSMailType data_type, FS_BOOL is_ui, const wchar_t* to,
  const wchar_t* subject, const wchar_t* cc, const wchar_t* bcc, const wchar_t* message) {
  return FALSE;
}

FS_UINT32 gVerifySignature(void* user_data, const FS_PDFDOC_HANDLE* document, const FS_SIGNATURE_HANDLE* pdf_signature) {
  return e_FSStatesStateUnknown;
}

FS_WSTR gGetUntitledBookmarkName(void* user_data) {
  InitFWSTR(&gaction_callback_data.untitled_bookmark_name_, L"Untitled");
  return gaction_callback_data.untitled_bookmark_name_;
}

void gGetPrinterNameList(void* user_data, FS_WSTR* return_stringarray, FS_UINT32* return_stringarray_length) {
  return;
}

FS_BOOL gAddToolButton(void* user_data, const FSButtonItem button_item) {
  return FALSE;
}

FS_BOOL gRemoveToolButtom(void* user_data, const wchar_t* button_name) {
  return FALSE;
}

void gGetMenuItemNameList(void* user_data, FSMenuList* return_menulistarray, FS_UINT32* return_menulistarray_length) {
  return;
}

FS_BOOL gAddMenuItem(void* user_data, const FSMenuItemConfig menu_item_config, FS_BOOL is_prepend) {
  return FALSE;
}

FS_BOOL gAddSubMenu(void* user_data, const FSMenuItemConfig menu_item_config) {
  return FALSE;
}

FS_BOOL gShowDialog(void* user_data, const FSDialogDescriptionConfig dlg_config) {
  return FALSE;
}

FS_BOOL gGetFullScreen(void* user_data) {
  return FALSE;
}

void gSetFullScreen(void* user_data, FS_BOOL is_full_screen) {
  return;
}

void gOnFieldValueChanged(void *user_data, const wchar_t* field_name, FSJSFieldValueChangeType type, const FS_WSTR *value_before_changed, FS_UINT32 value_before_changed_array_length_, const FS_WSTR *value_after_changed, FS_UINT32 value_after_changed_array_length_) {
  return;
}

void gUpdateLogicalLabel(void* user_data) {}

FSJsMailResult gMailDoc(void* user_data, const FS_PDFDOC_HANDLE* document,
  const wchar_t* to_address, const wchar_t* cc_address, const wchar_t* bcc_address,
  const wchar_t* subject, const wchar_t* message, FS_BOOL is_ui) {
  return (FSJsMailResult)0;
}

FS_WSTR gGetTemporaryFileName(void* user_data, const FS_PDFDOC_HANDLE* document, const wchar_t* file_name) {
  FSDK_WStr_Init(&gaction_callback_data.temporary_file_name_);
  return gaction_callback_data.temporary_file_name_;
}

FSMediaPlayerCallback* gOpenMediaPlayer(void* user_data, const FSPlayerArgs player_args) {
  return (void*)0;
}

FS_WSTR gGetTemporaryDirectory(void* user_data) {
  FSDK_WStr_Init(&gaction_callback_data.temporary_directory_);
  return gaction_callback_data.temporary_directory_;
}

void gScroll(void* user_data, const FSPointF point) { }

void gSelectPageNthWord(void* user_data, int page_index, int start_offset, int end_offset, FS_BOOL is_show_selection) { }

FSPointF gGetMousePosition(void* user_data) {
  FSPointF pointf;
  pointf.x = 0;
  pointf.y = 0;
  return pointf;
}

FSRectF gGetPageWindowRect(void* user_data) {
  FSRectF rectf;
  rectf.left = 0;
  rectf.right = 100;
  rectf.top = 100;
  rectf.bottom = 0;
  return rectf;
}

FSLayoutMode gGetLayoutMode(void* user_data) {
  return (FSLayoutMode)0;
}

void gSetLayoutMode(void* user_data, FSLayoutMode layout_mode, FS_BOOL is_cover_mode) { }

float gGetPageScale(void* user_data) {
  return 0.0f;
}

void gSetPageScale(void* user_data, FSZoomMode zoom_mode, const FS_DESTINATION_HANDLE* dest) { }

FSZoomMode gGetPageZoomMode(void* user_data) {
  return (FSZoomMode)0;
}

void gQuery(void* user_data, const wchar_t* keywords, FSSearchScope search_scope, const FSSearchOption* search_option, const wchar_t* di_path) {}

FSSearchIndexConfig gAddSearchIndex(void* user_data, const wchar_t* di_path, FS_BOOL is_selected) {
  FSSearchIndexConfig search_index_config;
  search_index_config.is_available = FALSE;
  search_index_config.is_selected = FALSE;
  FSDK_WStr_Init(&search_index_config.name);
  FSDK_WStr_Init(&search_index_config.path);    
  return search_index_config;
}
FS_BOOL gRemoveSearchIndex(void* user_data, const FSSearchIndexConfig* search_index_config) { return 0; }

void gGetSignatureAPStyleNameList(void* user_data, FS_WSTR* return_stringarray, FS_UINT32* return_stringarray_length) {
  return;
}

FSSOAPResponseInfo gSoapRequest(const FSSOAPRequestProperties* request_params) {
  FSSOAPResponseInfo soap_response_info;   
  return soap_response_info;
}

void AddAnnotations(FS_PDFPAGE_HANDLE page) {
  wchar_t uid[37];
  int uid_size = 37;
  {
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE line = NULL;
    FSPointF pointf;
    FS_BOOL result_resetap = 0;
    // Add line annotation 
    // No special intent, as a common line.
    FSRectF rectf;
    rectf.left = 0;
    rectf.bottom = 650;
    rectf.right = 100;
    rectf.top = 750;

    FSDK_PDFPage_AddAnnot(page, e_FSLine, rectf, &annot);    
    FSDK_Line_Create0(annot, &line);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(line, 4);
    pointf.x = 20;
    pointf.y = 650;
    FSDK_Line_SetStartPoint(line, pointf);
    pointf.x = 100;
    pointf.y = 740;
    FSDK_Line_SetEndPoint(line, pointf);
    FSDK_Annot_SetContent(line, L"A line arrow annotation");
    FSDK_Markup_SetIntent(line, "LineArrow");
    FSDK_Markup_SetSubject(line, L"Arrow");
    FSDK_Markup_SetTitle(line, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(line, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(line, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(line, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(line, &result_resetap);
    FSDK_Line_Release(line);
    FSDK_Annot_Release(annot);

    rectf.left = 0;
    rectf.bottom = 650;
    rectf.right = 100;
    rectf.top = 760;

    FSDK_PDFPage_AddAnnot(page, e_FSLine, rectf, &annot);
    FSDK_Line_Create0(annot, &line);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(line, 4);
    FSDK_Annot_SetBorderColor(line, 0x00FF00);
    pointf.x = 10;
    pointf.y = 650;
    FSDK_Line_SetStartPoint(line, pointf);
    pointf.x = 100;
    pointf.y = 750;
    FSDK_Line_SetEndPoint(line, pointf);
    FSDK_Annot_SetContent(line, L"A common line.");
    FSDK_Line_SetLineStartStyle(line, e_FSEndingStyleSquare);
    FSDK_Line_SetLineEndStyle(line, e_FSEndingStyleOpenArrow);
    // Show text in line
    FSDK_Line_EnableCaption(line, 1);
    pointf.x = 0;
    pointf.y = 5;
    FSDK_Line_SetCaptionOffset(line, pointf);
    FSDK_Markup_SetSubject(line, L"Line");
    FSDK_Markup_SetTitle(line, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(line, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(line, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(line, (uid, WRandomUID(uid, uid_size)));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(line, &result_resetap);
    FSDK_Line_Release(line);
    FSDK_Annot_Release(annot);
    printf("Add a line annotations.\r\n");
  }

  {
    // Add circle annotation
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE circle = NULL;
    FS_BOOL result_resetap = 0;
    FSRectF rectf;
    rectf.left = 100;
    rectf.bottom = 650;
    rectf.right = 200;
    rectf.top = 750;

    FSDK_PDFPage_AddAnnot(page, e_FSCircle, rectf, &annot);    
    FSDK_Circle_Create0(annot, &circle);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(circle, 4);

    rectf.left = 120;
    rectf.bottom = 660;
    rectf.right = 160;
    rectf.top = 740;

    FSDK_Circle_SetInnerRect(circle, rectf);
    FSDK_Markup_SetSubject(circle, L"Circle");
    FSDK_Markup_SetTitle(circle, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(circle, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(circle, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(circle, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(circle, &result_resetap);
    FSDK_Circle_Release(circle);
    FSDK_Annot_Release(annot);
    printf("Add a circle annotation.\r\n");
  }

  {
    // Add square annotation
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE square = NULL;
    FS_BOOL result_resetap = FALSE;
    FSRectF rectf;
    rectf.left = 100;
    rectf.bottom = 650;
    rectf.right = 300;
    rectf.top = 750;

    FSDK_PDFPage_AddAnnot(page, e_FSSquare, rectf, &annot);    
    FSDK_Square_Create0(annot, &square);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(square, 4);

    FSDK_Square_SetFillColor(square, 0x00FF00);

    rectf.left = 220;
    rectf.bottom = 660;
    rectf.right = 260;
    rectf.top = 740;

    FSDK_Square_SetInnerRect(square, rectf);
    FSDK_Markup_SetSubject(square, L"Square");
    FSDK_Markup_SetTitle(square, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(square, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(square, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(square, WRandomUID(uid, uid_size));

    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(square, &result_resetap);
    FSDK_Square_Release(square);
    FSDK_Annot_Release(annot);
    printf("Add a square annotation.\r\n");
  }

  {
    // Add polygon annotation, as cloud.
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE polygon = NULL;
    FSPointF* pointfarray = NULL;
    FS_BOOL result_resetap = 0;
    FSRectF rectf;
    rectf.left = 300;
    rectf.bottom = 650;
    rectf.right = 500;
    rectf.top = 750;

    FSDK_PDFPage_AddAnnot(page, e_FSPolygon, rectf, &annot);    
    FSDK_Polygon_Create0(annot, &polygon);

    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(polygon, 4);

    FSDK_Markup_SetIntent(polygon, "PolygonCloud");
    FSDK_Polygon_SetFillColor(polygon, 0x0000FF);

    pointfarray = (FSPointF*)malloc(6 * sizeof(FSPointF));
    pointfarray[0].x = 335;
    pointfarray[0].y = 665;
    pointfarray[1].x = 365;
    pointfarray[1].y = 665;
    pointfarray[2].x = 385;
    pointfarray[2].y = 705;
    pointfarray[3].x = 365;
    pointfarray[3].y = 740;
    pointfarray[4].x = 335;
    pointfarray[4].y = 740;
    pointfarray[5].x = 315;
    pointfarray[5].y = 705;
    FSDK_Polygon_SetVertexes(polygon, pointfarray, 6);
    FSDK_Markup_SetSubject(polygon, L"Cloud");
    FSDK_Markup_SetTitle(polygon, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(polygon, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(polygon, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(polygon, WRandomUID(uid, uid_size));

    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(polygon, &result_resetap);
    FSDK_Polygon_Release(polygon);
    FSDK_Annot_Release(annot);
    free(pointfarray);
    printf("Add a polygon cloud annotation.\r\n");
  }

  {
    // Add polygon annotation, with dashed border.
    FSBorderInfo borderinfo;
    FSRectF rectf;
    FSPointF* pointfarray;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE polygon = NULL;
    FS_BOOL result_resetap = FALSE;
    borderinfo.cloud_intensity = 2.0f;
    borderinfo.width = 2.0f;
    borderinfo.style = e_FSDashed;
    borderinfo.dash_phase = 3.f;
    borderinfo.dash_array = (float*)malloc(2 * sizeof(float));
    borderinfo.dash_array[0] = 2;
    borderinfo.dash_array[1] = 2;
    borderinfo.array_length_dashes = 2;

    rectf.left = 400;
    rectf.bottom = 650;
    rectf.right = 500;
    rectf.top = 750;

    FSDK_PDFPage_AddAnnot(page, e_FSPolygon, rectf, &annot);
    FSDK_Polygon_Create0(annot, &polygon);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(polygon, 4);

    FSDK_Polygon_SetFillColor(polygon, 0x0000FF);
    FSDK_Annot_SetBorderInfo(polygon, borderinfo);

    pointfarray = (FSPointF*)malloc(6 * sizeof(FSPointF));
    pointfarray[0].x = 435;
    pointfarray[0].y = 665;
    pointfarray[1].x = 465;
    pointfarray[1].y = 665;
    pointfarray[2].x = 485;
    pointfarray[2].y = 705;
    pointfarray[3].x = 465;
    pointfarray[3].y = 740;
    pointfarray[4].x = 435;
    pointfarray[4].y = 740;
    pointfarray[5].x = 415;
    pointfarray[5].y = 705;
    FSDK_Polygon_SetVertexes(polygon, pointfarray, 6);
    FSDK_Markup_SetSubject(polygon, L"Polygon");
    FSDK_Markup_SetTitle(polygon, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(polygon, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(polygon, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(polygon, WRandomUID(uid, uid_size));

    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(polygon, &result_resetap);
    FSDK_Polygon_Release(polygon);
    FSDK_Annot_Release(annot);
    free(pointfarray);
    free(borderinfo.dash_array);

    printf("Add a polygon annotation.\r\n");
  }

  {
    // Add polyline annotation
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE polyline = NULL;
    FSPointF* pointfarray = NULL;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 500;
    rectf.bottom = 650;
    rectf.right = 600;
    rectf.top = 700;

    FSDK_PDFPage_AddAnnot(page, e_FSPolyLine, rectf, &annot);    
    FSDK_PolyLine_Create0(annot, &polyline);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(polyline, 4);

    pointfarray = (FSPointF*)malloc(6 * sizeof(FSPointF));
    pointfarray[0].x = 515;
    pointfarray[0].y = 705;
    pointfarray[1].x = 535;
    pointfarray[1].y = 740;
    pointfarray[2].x = 565;
    pointfarray[2].y = 740;
    pointfarray[3].x = 585;
    pointfarray[3].y = 705;
    pointfarray[4].x = 565;
    pointfarray[4].y = 665;
    pointfarray[5].x = 535;
    pointfarray[5].y = 665;
    FSDK_PolyLine_SetVertexes(polyline, pointfarray, 6);

    FSDK_Markup_SetSubject(polyline, L"PolyLine");
    FSDK_Markup_SetTitle(polyline, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(polyline, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(polyline, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(polyline, WRandomUID(uid, uid_size));

    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(polyline, &result_resetap);
    FSDK_PolyLine_Release(polyline);
    FSDK_Annot_Release(annot);
    free(pointfarray);
    printf("Add a polyline annotation.\r\n");
  }

  {
    // Add freetext annotation, as type writer.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE freetext = NULL;
    FSDefaultAppearance default_ap;
    FS_BOOL result_setdefaultap = FALSE;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 10;
    rectf.bottom = 550;
    rectf.right = 200;
    rectf.top = 600;
    
    FSDK_PDFPage_AddAnnot(page, e_FSFreeText, rectf, &annot);

    FSDK_FreeText_Create0(annot, &freetext);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(freetext, 4);

    // Set default appearance
    default_ap.flags = e_FSFlagFont | e_FSFlagFontSize | e_FSFlagTextColor;
    FSDK_Font_Create0(e_FSStdIDHelvetica, &default_ap.font);

    default_ap.text_size = 12.0f;
    default_ap.text_color = 0x000000;
    // Set default appearance for form.
    FSDK_FreeText_SetDefaultAppearance(freetext, default_ap, &result_setdefaultap);

    FSDK_Markup_SetIntent(freetext, "FreeTextTypewriter");
    FSDK_Annot_SetContent(freetext, L"typewriter annotation");
    FSDK_Markup_SetSubject(freetext, L"FreeTextTypewriter");
    FSDK_Markup_SetTitle(freetext, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(freetext, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(freetext, &result_resetap);
    FSDK_FreeText_Release(freetext);
    FSDK_Annot_Release(annot);
    FSDK_Font_Release(default_ap.font);
    printf("Add a typewriter freetext annotation.\r\n");
  }

  {
    // Add freetext annotation, as callout.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE freetext = NULL;
    FSDefaultAppearance default_ap;
    FSPointF* pointfarray = NULL;
    FS_BOOL result_setdefaultap = FALSE;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 300;
    rectf.bottom = 550;
    rectf.right = 400;
    rectf.top = 600;

    FSDK_PDFPage_AddAnnot(page, e_FSFreeText, rectf, &annot);

    FSDK_FreeText_Create0(annot, &freetext);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(freetext, 4);

    // Set default appearance
    default_ap.flags = e_FSFlagFont | e_FSFlagFontSize | e_FSFlagTextColor;
    FSDK_Font_Create0(e_FSStdIDHelveticaB, &default_ap.font);

    default_ap.text_size = 12.0f;
    default_ap.text_color = 0x000000;
    // Set default appearance for freetext.    
    FSDK_FreeText_SetDefaultAppearance(freetext, default_ap, &result_setdefaultap);

    FSDK_Markup_SetIntent(freetext, "FreeTextCallout");

    pointfarray = (FSPointF*)malloc(6 * sizeof(FSPointF));
    pointfarray[0].x = 250;
    pointfarray[0].y = 540;
    pointfarray[1].x = 280;
    pointfarray[1].y = 570;
    pointfarray[2].x = 300;
    pointfarray[2].y = 570;
    FSDK_FreeText_SetCalloutLinePoints(freetext, pointfarray, 3);
    FSDK_FreeText_SetCalloutLineEndingStyle(freetext, e_FSEndingStyleOpenArrow);

    FSDK_Annot_SetContent(freetext, L"A callout annotation.");
    FSDK_Markup_SetSubject(freetext, L"FreeTextCallout");
    FSDK_Markup_SetTitle(freetext, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(freetext, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(freetext, &result_resetap);
    FSDK_FreeText_Release(freetext);
    FSDK_Annot_Release(annot);
    FSDK_Font_Release(default_ap.font);
    free(pointfarray);
    printf("Add a callout freetext annotation.\r\n");
  }

  {
    // Add freetext annotation, as text box.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE freetext = NULL;
    FSDefaultAppearance default_ap;
    FS_BOOL result_setdefaultap = FALSE;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 450;
    rectf.bottom = 550;
    rectf.right = 550;
    rectf.top = 600;
    
    FSDK_PDFPage_AddAnnot(page, e_FSFreeText, rectf, &annot);

    FSDK_FreeText_Create0(annot, &freetext);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(freetext, 4);

    // Set default appearance

    default_ap.flags = e_FSFlagFont | e_FSFlagFontSize | e_FSFlagTextColor;
    FSDK_Font_Create0(e_FSStdIDHelveticaI, &default_ap.font);

    default_ap.text_size = 12.0f;
    default_ap.text_color = 0x000000;
    // Set default appearance for freetext.
    FSDK_FreeText_SetDefaultAppearance(freetext, default_ap, &result_setdefaultap);

    FSDK_Annot_SetContent(freetext, L"A text box annotation.");
    FSDK_Markup_SetSubject(freetext, L"Textbox");
    FSDK_Markup_SetTitle(freetext, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(freetext, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(freetext, &result_resetap);
    FSDK_FreeText_Release(freetext);
    FSDK_Annot_Release(annot);
    FSDK_Font_Release(default_ap.font);

    printf("Add a text box freetext annotation.\r\n");
  }

  {
    // Add highlight annotation.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE textmarkup = NULL;
    FS_ANNOT_HANDLE highlight = NULL;
    FSQuadPoints* quad_points;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 10;
    rectf.bottom = 550;
    rectf.right = 100;
    rectf.top = 550;

    FSDK_PDFPage_AddAnnot(page, e_FSHighlight, rectf, &annot);

    FSDK_TextMarkup_Create0(annot, &textmarkup);    
    FSDK_Highlight_Create0(annot, &highlight);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(highlight, 4);

    FSDK_Annot_SetContent(highlight, L"Highlight");
    quad_points = (FSQuadPoints*)malloc(1 * sizeof(FSQuadPoints));
    quad_points[0].first.x = 10;
    quad_points[0].first.y = 500;
    quad_points[0].second.x = 90;
    quad_points[0].second.y = 500;
    quad_points[0].third.x = 10;
    quad_points[0].third.y = 480;
    quad_points[0].fourth.x = 90;
    quad_points[0].fourth.y = 480;
    FSDK_TextMarkup_SetQuadPoints(textmarkup, quad_points, 1);

    FSDK_Markup_SetSubject(highlight, L"Highlight");
    FSDK_Markup_SetTitle(highlight, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(highlight, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(highlight, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(highlight, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(highlight, &result_resetap);
    FSDK_Highlight_Release(highlight);
    FSDK_TextMarkup_Release(annot);
    FSDK_Annot_Release(annot);
    free(quad_points);
    printf("Add a highlight annotation.\r\n");
  }

  {
    // Add underline annotation.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE textmarkup = NULL;
    FS_ANNOT_HANDLE underline = NULL;
    FSQuadPoints* quad_points;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 100;
    rectf.bottom = 450;
    rectf.right = 200;
    rectf.top = 550;
    
    FSDK_PDFPage_AddAnnot(page, e_FSUnderline, rectf, &annot);

    FSDK_TextMarkup_Create0(annot, &textmarkup);    
    FSDK_Underline_Create0(annot, &underline);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(underline, 4);

    quad_points = (FSQuadPoints*)malloc(sizeof(FSQuadPoints));
    quad_points[0].first.x = 110;
    quad_points[0].first.y = 500;
    quad_points[0].second.x = 190;
    quad_points[0].second.y = 500;
    quad_points[0].third.x = 110;
    quad_points[0].third.y = 480;
    quad_points[0].fourth.x = 190;
    quad_points[0].fourth.y = 480;
    FSDK_TextMarkup_SetQuadPoints(textmarkup, quad_points, 1);

    FSDK_Markup_SetSubject(underline, L"Underline");
    FSDK_Markup_SetTitle(underline, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(underline, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(underline, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(underline, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(underline, &result_resetap);
    FSDK_Underline_Release(underline);
    FSDK_TextMarkup_Release(annot);
    FSDK_Annot_Release(annot);
    free(quad_points);
    printf("Add a underline annotation.\r\n");
  }
  {
    // Add squiggly annotation.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE textmarkup = NULL;
    FS_ANNOT_HANDLE squiggly = NULL;
    FSQuadPoints* quad_points;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 200;
    rectf.bottom = 450;
    rectf.right = 300;
    rectf.top = 550;
    
    FSDK_PDFPage_AddAnnot(page, e_FSSquiggly, rectf, &annot);

    FSDK_TextMarkup_Create0(annot, &textmarkup);    
    FSDK_Squiggly_Create0(annot, &squiggly);
    FSDK_Markup_SetIntent(annot, "Squiggly");
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(squiggly, 4);

    quad_points = (FSQuadPoints*)malloc(sizeof(FSQuadPoints));
    quad_points[0].first.x = 210;
    quad_points[0].first.y = 500;
    quad_points[0].second.x = 290;
    quad_points[0].second.y = 500;
    quad_points[0].third.x = 210;
    quad_points[0].third.y = 480;
    quad_points[0].fourth.x = 290;
    quad_points[0].fourth.y = 480;
    FSDK_TextMarkup_SetQuadPoints(textmarkup, quad_points, 1);

    FSDK_Markup_SetSubject(squiggly, L"Squiggly");
    FSDK_Markup_SetTitle(squiggly, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(squiggly, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(squiggly, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(squiggly, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(squiggly, &result_resetap);
    FSDK_Squiggly_Release(squiggly);
    FSDK_TextMarkup_Release(annot);
    FSDK_Annot_Release(annot);
    free(quad_points);

    printf("Add a squiggly annotation.\r\n");
  }
  {
    // Add strikeout annotation.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE textmarkup = NULL;
    FS_ANNOT_HANDLE strikeout = NULL;
    FSQuadPoints* quad_points;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 300;
    rectf.bottom = 450;
    rectf.right = 400;
    rectf.top = 550;

    FSDK_PDFPage_AddAnnot(page, e_FSStrikeOut, rectf, &annot);
    FSDK_TextMarkup_Create0(annot, &textmarkup);
    FSDK_StrikeOut_Create0(annot, &strikeout);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(strikeout, 4);

    quad_points  = (FSQuadPoints*)malloc(sizeof(FSQuadPoints));
    quad_points[0].first.x = 310;
    quad_points[0].first.y = 500;
    quad_points[0].second.x = 390;
    quad_points[0].second.y = 500;
    quad_points[0].third.x = 310;
    quad_points[0].third.y = 480;
    quad_points[0].fourth.x = 390;
    quad_points[0].fourth.y = 480;
    FSDK_TextMarkup_SetQuadPoints(textmarkup, quad_points, 1);

    FSDK_Markup_SetSubject(strikeout, L"StrikeOut");
    FSDK_Markup_SetTitle(strikeout, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(strikeout, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(strikeout, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(strikeout, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(strikeout, &result_resetap);
    FSDK_StrikeOut_Release(strikeout);
    FSDK_TextMarkup_Release(annot);
    FSDK_Annot_Release(annot);
    free(quad_points);
    printf("Add a strikeout annotation.\r\n");
  }

  {
    // Add caret annotation.
    FSRectF rectf;
    FS_ANNOT_HANDLE caret = NULL;
    FS_ANNOT_HANDLE annot = NULL;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 400;
    rectf.bottom = 450;
    rectf.right = 420;
    rectf.top = 470;

    FSDK_PDFPage_AddAnnot(page, e_FSCaret, rectf, &annot);
    FSDK_Caret_Create0(annot, &caret);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(caret, 4);

    rectf.left = 410;
    rectf.bottom = 450;
    rectf.right = 430;
    rectf.top = 470;
    FSDK_Caret_SetInnerRect(caret, rectf);

    FSDK_Annot_SetContent(caret, L"Caret annotation");
    FSDK_Markup_SetSubject(caret, L"Caret");
    FSDK_Markup_SetTitle(caret, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(caret, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(caret, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(caret, WRandomUID(uid, uid_size));
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(caret, &result_resetap);
    FSDK_Caret_Release(caret);
    FSDK_Annot_Release(annot);
    printf("Add a caret annotation.\r\n");
  }

  {
    // Add note annotation
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE note = NULL;
    FS_ANNOT_HANDLE annotpop = NULL;
    FS_ANNOT_HANDLE popup = NULL;
    FS_NOTEANNOT_HANDLE reply = NULL;
    FS_NOTEANNOT_HANDLE state = NULL;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 10;
    rectf.bottom = 350;
    rectf.right = 50;
    rectf.top = 400;
    
    FSDK_PDFPage_AddAnnot(page, e_FSNote, rectf, &annot);

    FSDK_Note_Create0(annot, &note);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(note, 4);

    FSDK_Note_SetIconName(note, "Comment");
    FSDK_Markup_SetSubject(note, L"Note");
    FSDK_Markup_SetTitle(note, L"Foxit SDK");
    FSDK_Annot_SetContent(note, L"Note annotation");
    FSDK_Markup_SetCreationDateTime(note, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(note, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(note, WRandomUID(uid, uid_size));

    // Add popup to note annotation
    rectf.left = 300;
    rectf.bottom = 450;
    rectf.right = 500;
    rectf.top = 550;

    FSDK_PDFPage_AddAnnot(page, e_FSPopup, rectf, &annotpop);
    FSDK_Popup_Create0(annot, &popup);
    FSDK_Annot_SetBorderColor(popup, 0x00FF00);
    FSDK_Popup_SetOpenStatus(popup, FALSE);
    FSDK_Annot_SetModifiedDateTime(popup, GetLocalDateTime());
    FSDK_Markup_SetPopup(note, popup);

    // Add reply annotation to note annotation

    FSDK_Markup_AddReply(note, &reply);
    FSDK_Annot_SetContent(reply, L"reply");
    FSDK_Annot_SetModifiedDateTime(reply, GetLocalDateTime());
    FSDK_Markup_SetTitle(reply, L"Foxit SDK");
    FSDK_Annot_SetUniqueID(reply, WRandomUID(uid, uid_size));

    // Add state annotation to note annotation
    FSDK_Markup_AddStateAnnot(note, L"Foxit SDK", e_FSStateModelReview, e_FSStateAccepted, &state);
    FSDK_Annot_SetContent(state, L"Accepted set by Foxit SDK");
    FSDK_Markup_SetTitle(state, L"Foxit SDK");
    FSDK_Annot_SetModifiedDateTime(state, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(state, WRandomUID(uid, uid_size));

    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(note, &result_resetap);
    FSDK_Note_Release(state);
    FSDK_Note_Release(reply);
    FSDK_Popup_Release(popup);
    FSDK_Annot_Release(annotpop);
    FSDK_Note_Release(note);
    FSDK_Annot_Release(annot);
    printf("Add a note annotation.\r\n");
  }

  {
    // Add ink annotation
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE ink = NULL;
    FS_PATH_HANDLE inklist = NULL;
    FSPointF center;
    FSPointF newpoint;
    float width;
    float height;
    float out_width;
    float inner_width;
    float x;
    float y;
    int i;
    FS_BOOL resultpath = FALSE;
    FS_BOOL result_resetap = FALSE;
    rectf.left = 100;
    rectf.bottom = 350;
    rectf.right = 200;
    rectf.top = 450;

    FSDK_PDFPage_AddAnnot(page, e_FSInk, rectf, &annot);


    FSDK_Ink_Create0(annot, &ink);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(ink, 4);  
    FSDK_Path_Create(&inklist);

    width = 100;
    height = 100;
    out_width = min(width, height) * 2 / 3.f;
    inner_width = (float)(out_width * sin(18.f / 180.f * 3.14f) / sin(36.f / 180.f * 3.14f));
    center.x = 150;
    center.y = 400;
    x = out_width;
    y = 0;
    newpoint.x = center.x + x;
    newpoint.y = center.y + y;
    FSDK_Path_MoveTo(inklist, newpoint, &resultpath);

    for (i = 0; i < 5; i++) {
      x = (float)(out_width * cos(72.f * i / 180.f * 3.14f));
      y = (float)(out_width * sin(72.f * i / 180.f * 3.14f));
      newpoint.x = center.x + x;
      newpoint.y = center.y + y;
      FSDK_Path_LineTo(inklist, newpoint, &resultpath);

      x = (float)(inner_width * cos((72.f * i + 36) / 180.f * 3.14f));
      y = (float)(inner_width * sin((72.f * i + 36) / 180.f * 3.14f));
      newpoint.x = center.x + x;
      newpoint.y = center.y + y;
      FSDK_Path_LineTo(inklist, newpoint, &resultpath);
    }
    newpoint.x = center.x + out_width;
    newpoint.y = center.y;
    FSDK_Path_LineTo(inklist, newpoint, &resultpath);
    FSDK_Path_CloseFigure(inklist, &resultpath);
    FSDK_Ink_SetInkList(ink, inklist);

    FSDK_Annot_SetContent(ink, L"Ink annotation");
    FSDK_Markup_SetSubject(ink, L"Ink");
    FSDK_Markup_SetTitle(ink, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(ink, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(ink, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(ink, WRandomUID(uid, uid_size));

    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(ink, &result_resetap);

    FSDK_Path_Release(inklist);
    FSDK_Ink_Release(ink);
    FSDK_Annot_Release(annot);
    printf("Add an ink annotation.\r\n");
  }

  {
    // Add file attachment annotation
    wchar_t pdf_file[MAX_FILE_PATH] = { 0 }; 
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE file_attachment = NULL;
    FS_PDFDOC_HANDLE doc = NULL;
    FS_FILESPEC_HANDLE file_spec = NULL;
    FS_BOOL result = FALSE;
    FS_BOOL result_resetap = FALSE;
    swprintf_s(pdf_file, MAX_FILE_PATH, L"%sAboutFoxit.pdf", input_path);
    rectf.left = 280;
    rectf.bottom = 350;
    rectf.right = 300;
    rectf.top = 380;
    
    FSDK_PDFPage_AddAnnot(page, e_FSFileAttachment, rectf, &annot);    
    FSDK_FileAttachment_Create0(annot, &file_attachment);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(file_attachment, 4);

    FSDK_FileAttachment_SetIconName(file_attachment, "Graph");
    
    FSDK_PDFPage_GetDocument(page, &doc);    
    FSDK_FileSpec_Create(doc, &file_spec);
    FSDK_FileSpec_SetFileName(file_spec, L"attachment.pdf");
    FSDK_FileSpec_SetCreationDateTime(file_spec, GetLocalDateTime());
    FSDK_FileSpec_SetDescription(file_spec, "The original file");
    FSDK_FileSpec_SetModifiedDateTime(file_spec, GetLocalDateTime());    
    FSDK_FileSpec_Embed(file_spec, pdf_file, &result);

    FSDK_FileAttachment_SetFileSpec(file_attachment, file_spec, &result);
    FSDK_Markup_SetSubject(file_attachment, L"File Attachment");
    FSDK_Markup_SetTitle(file_attachment, L"Foxit SDK");

    // Appearance should be reset.    
    FSDK_Annot_ResetAppearanceStream(file_attachment, &result_resetap);

    FSDK_FileSpec_Release(file_spec);
    FSDK_FileAttachment_Release(file_attachment);
    FSDK_Annot_Release(annot);
    FSDK_PDFDoc_Release(doc);
    printf("Add an attachment annotation.\r\n");
  }
  {
    // Add link annotation
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE link = NULL;
    FS_PDFDOC_HANDLE doc = NULL;
    FS_ACTION_HANDLE action = NULL;
    FS_URIACTION_HANDLE uriaction = NULL;
    rectf.left = 350;
    rectf.bottom = 350;
    rectf.right = 380;
    rectf.top = 400;
    
    FSDK_PDFPage_AddAnnot(page, e_FSLink, rectf, &annot);    
    FSDK_Link_Create0(annot, &link);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(link, 4);

    FSDK_Link_SetHighlightingMode(link, e_FSHighlightingPush);

    // Add action for link annotation    
    FSDK_PDFPage_GetDocument(page, &doc);    
    FSDK_Action_Create(doc, e_FSTypeURI, &action);    
    FSDK_URIAction_Create(action, &uriaction);

    FSDK_URIAction_SetTrackPositionFlag(uriaction, TRUE);
    FSDK_URIAction_SetURI(uriaction, "www.foxitsoftware.com");
    FSDK_Link_SetAction(link, uriaction);
    // Appearance should be reset.

    FSDK_Action_Release(uriaction);
    FSDK_Link_Release(link);
    FSDK_Annot_Release(annot);
    FSDK_PDFDoc_Release(doc);
    printf("Add a link annotation.\r\n");
  }

  {
    // Set icon provider for annotation to Foxit PDF SDK.
    FS_BOOL result = FALSE;
    FSIconProviderCallback* icon_provider;
    FSRectF rectf;
    FS_ANNOT_HANDLE static_stamp_annot = NULL;
    FS_ANNOT_HANDLE static_stamp = NULL;
    FS_PDFDOC_HANDLE doc_handle = NULL;
    FSActionCallback* actionevent;
    FS_ANNOT_HANDLE dynamic_stamp_annot = NULL;
    FS_ANNOT_HANDLE dynamic_stamp = NULL;
    //need init
    gicon_provider_data.use_dynamic_stamp_ = FALSE;
    sprintf_s(gicon_provider_data.file_folder_, MAX_FILE_PATH, "../input_files/Stamps");
    gicon_provider_data.provider_id.str = NULL;
    gicon_provider_data.provider_version.str = NULL;
    gicon_provider_data.pdf_doc_map_ = hash_map_create();
    
    icon_provider = (FSIconProviderCallback*)malloc(sizeof(FSIconProviderCallback));
    icon_provider->user_data = icon_provider;
    icon_provider->Release = gRelease;
    icon_provider->CanChangeColor = gCanChangeColor;
    icon_provider->GetDisplayHeight = gGetDisplayHeight;
    icon_provider->GetDisplayWidth = gGetDisplayWidth;
    icon_provider->GetIcon = gGetIcon;
    icon_provider->GetProviderID = gGetProviderID;
    icon_provider->GetProviderVersion = gGetProviderVersion;
    icon_provider->GetShadingColor = gGetShadingColor;
    icon_provider->HasIcon = gHasIcon;
    FSDK_Library_SetAnnotIconProviderCallback(icon_provider, &result);

    // Add common stamp annotation.
    FSDK_Library_SetActionCallback(NULL, &result);    
    rectf.left = 110;
    rectf.bottom = 150;
    rectf.right = 200;
    rectf.top = 250;

    FSDK_PDFPage_AddAnnot(page, e_FSStamp, rectf, &static_stamp_annot);    
    FSDK_Stamp_Create0(static_stamp_annot, &static_stamp);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(static_stamp, 4);

    FSDK_Stamp_SetIconName(static_stamp, "Approved");
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(static_stamp, &result);
    FSDK_Stamp_Release(static_stamp);
    FSDK_Annot_Release(static_stamp_annot);

    // Add dynamic stamp annotation.    
    gicon_provider_data.use_dynamic_stamp_ = TRUE;
    FSDK_PDFPage_GetDocument(page, &doc_handle);
    gaction_callback_data.current_doc_ = doc_handle;
    FSDK_WStr_Init(&gaction_callback_data.app_info);
    FSDK_WStr_Init(&gaction_callback_data.attachments_file_path_);
    FSDK_WStr_Init(&gaction_callback_data.extracted_embedded_file_path_);
    FSDK_WStr_Init(&gaction_callback_data.browse_file0_);
    FSDK_WStr_Init(&gaction_callback_data.browse_file0_);
    FSDK_WStr_Init(&gaction_callback_data.file_path_);
    FSDK_WStr_Init(&gaction_callback_data.maildoc_);
    FSDK_WStr_Init(&gaction_callback_data.popup_menu_);
    FSDK_WStr_Init(&gaction_callback_data.response_);
    FSDK_WStr_Init(&gaction_callback_data.temporary_directory_);
    FSDK_WStr_Init(&gaction_callback_data.temporary_file_name_);
    FSDK_WStr_Init(&gaction_callback_data.untitled_bookmark_name_);

    actionevent = (FSActionCallback*)malloc(sizeof(FSActionCallback));
    actionevent->user_data = actionevent;
    actionevent->Release = gActionRelease;
    actionevent->InvalidateRect = gInvalidateRect;
    actionevent->GetCurrentPage = gGetCurrentPage;
    actionevent->SetCurrentPage = gSetCurrentPage;
    actionevent->SetCurrentPage0 = gSetCurrentPage0;
    actionevent->GetPageRotation = gGetPageRotation;
    actionevent->SetPageRotation = gSetPageRotation;
    actionevent->ExecuteNamedAction = gExecuteNamedAction;
    actionevent->SetDocChangeMark = gSetDocChangeMark;
    actionevent->GetDocChangeMark = gGetDocChangeMark;
    actionevent->GetOpenedDocCount = gGetOpenedDocCount;
    actionevent->GetOpenedDoc = gGetOpenedDoc;
    actionevent->GetCurrentDoc = gGetCurrentDoc;
    actionevent->CreateBlankDoc = gCreateBlankDoc;
    actionevent->OpenDoc = gOpenDoc;
    actionevent->CloseDoc = gCloseDoc;
    actionevent->Beep = gBeep;
    actionevent->FSResponse = gResponse;
    actionevent->GetFilePath = gGetFilePath;
    actionevent->IsLocalFile = gIsLocalFile;
    actionevent->GetAttachmentsFilePath = gGetAttachmentsFilePath;
    actionevent->GetExtractedEmbeddedFilePath = gGetExtractedEmbeddedFilePath;
    actionevent->Print0 = gPrint0;
    actionevent->Print = gPrint;
    actionevent->SubmitForm = gSubmitForm;
    actionevent->LaunchURL = gLaunchURL;
    actionevent->BrowseFile = gBrowseFile;
    actionevent->BrowseFile0 = gBrowseFile0;
    actionevent->GetLanguage = gGetLanguage;
    actionevent->Alert = gAlert;
    actionevent->GetIdentityProperties = gGetIdentityProperties;
    actionevent->SetIdentityProperties = gSetIdentityProperties;
    actionevent->PopupMenu = gPopupMenu;
    actionevent->PopupMenuEx = gPopupMenuEx;
    actionevent->GetAppInfo = gGetAppInfo;
    actionevent->MailData = gMailData;
    actionevent->VerifySignature = gVerifySignature;
    actionevent->GetUntitledBookmarkName = gGetUntitledBookmarkName;
    actionevent->GetPrinterNameList = gGetPrinterNameList;
    actionevent->AddToolButton = gAddToolButton;
    actionevent->RemoveToolButtom = gRemoveToolButtom;
    actionevent->GetMenuItemNameList = gGetMenuItemNameList;
    actionevent->AddMenuItem = gAddMenuItem;
    actionevent->AddSubMenu = gAddSubMenu;
    actionevent->ShowDialog = gShowDialog;
    actionevent->GetFullScreen = gGetFullScreen;
    actionevent->SetFullScreen = gSetFullScreen;
    actionevent->OnFieldValueChanged = gOnFieldValueChanged;
    actionevent->UpdateLogicalLabel = gUpdateLogicalLabel;
    actionevent->MailDoc = gMailDoc;
    actionevent->GetTemporaryFileName = gGetTemporaryFileName;
    actionevent->OpenMediaPlayer = gOpenMediaPlayer;
    actionevent->GetTemporaryDirectory = gGetTemporaryDirectory;
    actionevent->Scroll = gScroll;
    actionevent->SelectPageNthWord = gSelectPageNthWord;
    actionevent->GetMousePosition = gGetMousePosition;
    actionevent->GetPageWindowRect = gGetPageWindowRect;
    actionevent->GetLayoutMode = gGetLayoutMode;
    actionevent->SetLayoutMode = gSetLayoutMode;
    actionevent->GetPageScale = gGetPageScale;
    actionevent->SetPageScale = gSetPageScale;
    actionevent->GetPageZoomMode = gGetPageZoomMode;
    actionevent->Query = gQuery;
    actionevent->AddSearchIndex = gAddSearchIndex;
    actionevent->RemoveSearchIndex = gRemoveSearchIndex;
    actionevent->GetSignatureAPStyleNameList = gGetSignatureAPStyleNameList;
    FSDK_Library_SetActionCallback(actionevent, &result);
    rectf.left = 10;
    rectf.bottom = 150;
    rectf.right = 100;
    rectf.top = 250;

    FSDK_PDFPage_AddAnnot(page, e_FSStamp, rectf, &dynamic_stamp_annot);
    FSDK_Stamp_Create0(dynamic_stamp_annot, &dynamic_stamp);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(dynamic_stamp, 4);
    FSDK_Stamp_SetIconName(dynamic_stamp, "Approved");
    // Appearance should be reset.
    FSDK_Annot_ResetAppearanceStream(dynamic_stamp, &result);
    FSDK_Stamp_Release(dynamic_stamp);
    //FSDK_Annot_Release(dynamic_stamp_annot);

    printf("Add stamp annotations.\r\n");
  }

  {
    // Add freetext annotation with richtext, as type writer.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE freetext = NULL;
    FSDefaultAppearance default_ap;
    FS_BOOL result_setdefaultap = FALSE;
    FSRichTextStyle richtext_style;
    FS_FONT_HANDLE font0 = NULL;
    FS_FONT_HANDLE font1 = NULL;
    FS_BOOL result = 0;
    int richtext_count = 0;
    rectf.left = 10;
    rectf.bottom = 50;
    rectf.right = 200;
    rectf.top = 100;
    
    FSDK_PDFPage_AddAnnot(page, e_FSFreeText, rectf, &annot);

    FSDK_FreeText_Create0(annot, &freetext);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(freetext, 4);

    // Set default appearance    
    default_ap.flags = e_FSFlagFont | e_FSFlagFontSize | e_FSFlagTextColor;
    FSDK_Font_Create0(e_FSStdIDHelveticaI, &default_ap.font);

    default_ap.text_size = 12.0f;
    default_ap.text_color = 0x000000;
    // Set default appearance for freetext.    
    FSDK_FreeText_SetDefaultAppearance(freetext, default_ap, &result_setdefaultap);
    FSDK_Markup_SetIntent(freetext, "FreeTextTypewriter");
    FSDK_Annot_SetContent(freetext, L"A typewriter annotation");
    FSDK_Markup_SetSubject(freetext, L"FreeTextTypewriter");
    FSDK_Markup_SetTitle(freetext, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(freetext, WRandomUID(uid, uid_size));

    FSDK_Font_Create(L"Times New Roman", 0, e_FSCharsetANSI, 0, &font0);
    richtext_style.font = font0;
    richtext_style.text_color = 0xFF0000;
    richtext_style.text_alignment = e_FSAlignmentLeft;
    richtext_style.mark_style = e_FSCornerMarkNone;
    richtext_style.text_size = 10;
    richtext_style.is_bold = 0;
    richtext_style.is_italic = 0;
    richtext_style.is_strikethrough = 0;
    richtext_style.is_underline = 0;
    FSDK_Markup_AddRichText(freetext, L"Typewriter annotation ", richtext_style);

    richtext_style.text_color = 0x00FF00;
    richtext_style.is_underline = 1;
    FSDK_Markup_AddRichText(freetext, L"1-underline ", richtext_style);
    
    FSDK_Font_Create(L"Calibri", 0, e_FSCharsetANSI, 0, &font1);
    richtext_style.font = font1;
    richtext_style.text_color = 0x0000FF;
    richtext_style.is_underline = 0;
    richtext_style.is_strikethrough = 1;
    
    FSDK_Markup_GetRichTextCount(freetext, &richtext_count);
    FSDK_Markup_InsertRichText(freetext, richtext_count - 1, L"2_strikethrough ", richtext_style);

    FSDK_Font_Release(default_ap.font);

    // Appearance should be reset.    
    FSDK_Annot_ResetAppearanceStream(freetext, &result);
    printf("Add a typewriter freetext annotation with richtext.\r\n");
  }
  {
    // Add freetext annotation with richtext, as call-out.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE freetext = NULL;
    FSDefaultAppearance default_ap;
    FS_BOOL result_setdefaultap = FALSE;
    FSPointF* pointfarray;
    FSRichTextStyle richtext_style;
    FS_FONT_HANDLE font0 = NULL;
    FS_FONT_HANDLE font1;
    FS_BOOL result = 0;
    int richtext_count = 0;    
    rectf.left = 300;
    rectf.bottom = 50;
    rectf.right = 400;
    rectf.top = 100;
    
    FSDK_PDFPage_AddAnnot(page, e_FSFreeText, rectf, &annot);    
    FSDK_FreeText_Create0(annot, &freetext);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(freetext, 4);

    // Set default appearance    
    default_ap.flags = e_FSFlagFont | e_FSFlagFontSize | e_FSFlagTextColor;
    FSDK_Font_Create0(e_FSStdIDHelveticaI, &default_ap.font);

    default_ap.text_size = 12.0f;
    default_ap.text_color = 0x000000;
    // Set default appearance for freetext.
    FSDK_FreeText_SetDefaultAppearance(freetext, default_ap, &result_setdefaultap);
    FSDK_Markup_SetIntent(freetext, "FreeTextCallout");
    FSDK_Annot_SetContent(freetext, L"A callout annotation");
    FSDK_Markup_SetSubject(freetext, L"FreeTextCallout");
    FSDK_Markup_SetTitle(freetext, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(freetext, WRandomUID(uid, uid_size));

    pointfarray = (FSPointF*)malloc(6 * sizeof(FSPointF));
    pointfarray[0].x = 250;
    pointfarray[0].y = 60;
    pointfarray[1].x = 280;
    pointfarray[1].y = 80;
    pointfarray[2].x = 300;
    pointfarray[2].y = 80;
    FSDK_FreeText_SetCalloutLinePoints(freetext, pointfarray, 3);
    FSDK_FreeText_SetCalloutLineEndingStyle(freetext, e_FSEndingStyleOpenArrow);

    FSDK_Font_Create(L"Times New Roman", 0, e_FSCharsetANSI, 0, &font0);
    richtext_style.font = font0;
    richtext_style.text_color = 0xFF0000;
    richtext_style.text_size = 10;
    richtext_style.text_alignment = e_FSAlignmentLeft;
    richtext_style.mark_style = e_FSCornerMarkNone;
    richtext_style.is_bold = 0;
    richtext_style.is_italic = 0;
    richtext_style.is_strikethrough = 0;
    richtext_style.is_underline = 0;
    FSDK_Markup_AddRichText(freetext, L"Callout annotation ", richtext_style);

    richtext_style.text_color = 0x00FF00;
    richtext_style.is_underline = TRUE;
    FSDK_Markup_AddRichText(freetext, L"1-underline ", richtext_style);

    FSDK_Font_Create(L"Calibri", 0, e_FSCharsetANSI, 0, &font1);
    richtext_style.font = font1;
    richtext_style.text_color = 0x0000FF;
    richtext_style.is_underline = 0;
    richtext_style.is_strikethrough = 1;    
    FSDK_Markup_GetRichTextCount(freetext, &richtext_count);
    FSDK_Markup_InsertRichText(freetext, richtext_count - 1, L"2_strikethrough ", richtext_style);

    FSDK_Font_Release(default_ap.font);

    // Appearance should be reset.    
    FSDK_Annot_ResetAppearanceStream(freetext, &result);
    printf("Add a callout freetext annotation with richtext.\r\n");
  }

  {
    // Add freetext annotation with richtext, as text box.
    FSRectF rectf;
    FS_BOOL result = 0;
    FS_ANNOT_HANDLE annot = NULL;
    FS_ANNOT_HANDLE freetext = NULL;
    FSDefaultAppearance default_ap;
    FS_BOOL result_setdefaultap = FALSE;
    FSRichTextStyle richtext_style;
    FS_FONT_HANDLE font0 = NULL;
    FS_FONT_HANDLE font1 = NULL;
    int richtext_count = 0;
    rectf.left = 450;
    rectf.bottom = 50;
    rectf.right = 550;
    rectf.top = 100;

    FSDK_PDFPage_AddAnnot(page, e_FSFreeText, rectf, &annot);
    FSDK_FreeText_Create0(annot, &freetext);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(freetext, 4);

    // Set default appearance    
    default_ap.flags = e_FSFlagFont | e_FSFlagFontSize | e_FSFlagTextColor;
    FSDK_Font_Create0(e_FSStdIDHelveticaI, &default_ap.font);

    default_ap.text_size = 12.0f;
    default_ap.text_color = 0x000000;
    // Set default appearance for freetext.    
    FSDK_FreeText_SetDefaultAppearance(freetext, default_ap, &result_setdefaultap);
    FSDK_Annot_SetContent(freetext, L"A text box annotation");
    FSDK_Markup_SetSubject(freetext, L"Textbox");
    FSDK_Markup_SetTitle(freetext, L"Foxit SDK");
    FSDK_Markup_SetCreationDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetModifiedDateTime(freetext, GetLocalDateTime());
    FSDK_Annot_SetUniqueID(freetext, WRandomUID(uid, uid_size));

    FSDK_Font_Create(L"Times New Roman", 0, e_FSCharsetANSI, 0, &font0);
    richtext_style.font = font0;
    richtext_style.text_color = 0xFF0000;
    richtext_style.text_size = 10;
    richtext_style.text_alignment = e_FSAlignmentLeft;
    richtext_style.mark_style = e_FSCornerMarkNone;
    richtext_style.is_bold = 0;
    richtext_style.is_italic = 0;
    richtext_style.is_strikethrough = 0;
    richtext_style.is_underline = 0;
    FSDK_Markup_AddRichText(freetext, L"Textbox annotation ", richtext_style);

    richtext_style.text_color = 0x00FF00;
    richtext_style.is_underline = 1;
    FSDK_Markup_AddRichText(freetext, L"1-underline ", richtext_style);
    FSDK_Font_Create(L"Calibri", 0, e_FSCharsetANSI, 0, &font1);
    richtext_style.font = font1;
    richtext_style.text_color = 0x0000FF;
    richtext_style.is_underline = 0;
    richtext_style.is_strikethrough = 1;    
    FSDK_Markup_GetRichTextCount(freetext, &richtext_count);
    FSDK_Markup_InsertRichText(freetext, richtext_count - 1, L"2_strikethrough ", richtext_style);

    FSDK_Font_Release(default_ap.font);

    // Appearance should be reset.    
    FSDK_Annot_ResetAppearanceStream(freetext, &result);
    FSDK_Annot_Release(freetext);
    printf("Add a text box freetext annotation witch richtext.\r\n");
  }

  {
    //Add screen annotation.
    FSRectF rectf;
    FS_ANNOT_HANDLE annot = NULL;
    FS_SCREENANNOT_HANDLE screen = NULL;
    FS_IMAGE_HANDLE image = NULL;
    FS_ACTION_HANDLE action = NULL;
    FS_PDFDOC_HANDLE doc = NULL;
    wchar_t image_path[MAX_FILE_PATH] = { 0 };
    FSBorderInfo borderinfo;
    FS_RENDITION_HANDLE rendition = NULL;
    FS_RENDITIONACTION_HANDLE rendition_action = NULL;
    wchar_t video_file_path[MAX_FILE_PATH] = { 0 };
    FS_FILESPEC_HANDLE video_filespec = NULL;
    FS_BOOL return_value = FALSE;
    rectf.left = 300;
    rectf.bottom = 150;
    rectf.right = 400;
    rectf.top = 200;


    FSDK_PDFPage_AddAnnot(page, e_FSScreen, rectf, &annot);
    swprintf_s(image_path, MAX_FILE_PATH, L"%sFoxitLogo.jpg", input_path);
    FSDK_Image_Create0(image_path, &image);
    FSDK_Screen_Create0(annot, &screen);
    //This flag is used for printing annotations.
    FSDK_Annot_SetFlags(screen, 4);

    FSDK_Screen_SetTitle(screen, L"Foxit SDK");
    FSDK_Annot_SetBorderColor(screen, 0x0000FF);
    borderinfo.width = 2;
    borderinfo.style = e_FSSolid;
    borderinfo.cloud_intensity = 0;
    borderinfo.dash_phase = 0;
    borderinfo.dash_array = NULL;
    borderinfo.array_length_dashes = 0;
    FSDK_Annot_SetBorderInfo(screen, borderinfo);
    FSDK_Screen_SetImage(screen, image, 0, TRUE);

    //Prepare rendition action.

    FSDK_PDFPage_GetDocument(page, &doc);
    FSDK_Action_Create(doc, e_FSTypeRendition, &action);
    FSDK_RenditionAction_Create(action, &rendition_action);
    FSDK_RenditionAction_SetOperationType(rendition_action, e_FSOpTypeAssociate);
    FSDK_RenditionAction_SetScreenAnnot(rendition_action, screen);

    //Prepare rendition.
    FSDK_Rendition_Create(doc, NULL, &rendition);
    FSDK_Rendition_SetRenditionName(rendition, L"screen for rendition");
    FSDK_Rendition_SetMediaClipName(rendition, L"ytb210-Mopa.mp4");
    FSDK_Rendition_SetPermission(rendition, e_FSMediaPermTempAccess);
    FSDK_FileSpec_Create(doc, &video_filespec);
    FSDK_FileSpec_SetFileName(video_filespec, L"ytb210-Mopa.mp4");

    swprintf_s(video_file_path, MAX_FILE_PATH, L"%sytb210-Mopa.mp4", input_path);
    FSDK_FileSpec_Embed(video_filespec, video_file_path, &return_value);
    FSDK_Rendition_SetMediaClipFile(rendition, video_filespec);
    FSDK_Rendition_SetMediaClipContentType(rendition, "video/mp4");

    FSDK_RenditionAction_InsertRendition(rendition_action, rendition, -1);

    FSDK_Screen_SetAction(screen, rendition_action);
    FSDK_Annot_ResetAppearanceStream(screen, &return_value);
    printf("Add a screen annotation.\r\n");
  }
}

void AddAnnotsWithLowLevelAPI(FS_PDFPAGE_HANDLE page) {
  wchar_t uid[37];
  int uid_size = 37;
  {
    FSRectF rectf;
    // Add line annotation
    FS_PDFDICTIONARY_HANDLE annot_dict = NULL;
    FS_PDFARRAY_HANDLE coords_array = NULL;
    FS_PDFARRAY_HANDLE line_style = NULL;
    FS_PDFDICTIONARY_HANDLE border_dict = NULL;
    FS_PDFARRAY_HANDLE color_array = NULL;
    FS_ANNOT_HANDLE annot = NULL;
    FS_BOOL result_resetap = FALSE;
    FSDK_PDFDictionary_Create(&annot_dict);
    FSDK_PDFDictionary_SetAtName(annot_dict, "Type", "Annot");
    FSDK_PDFDictionary_SetAtName(annot_dict, "Subtype", "Line");
    FSDK_PDFDictionary_SetAtName(annot_dict, "IT", "LineArrow");
    FSDK_PDFDictionary_SetAtInteger(annot_dict, "F", 4);
    FSDK_PDFDictionary_SetAtString(annot_dict, "Contents", L"A line arrow annotation.");
    FSDK_PDFDictionary_SetAtString(annot_dict, "T", L"Foxit SDK");
    FSDK_PDFDictionary_SetAtString(annot_dict, "Subj", L"Arrow");
    FSDK_PDFDictionary_SetAtString(annot_dict, "NM", WRandomUID(uid, uid_size));
    FSDK_PDFDictionary_SetAtDateTime(annot_dict, "CreationDate", GetLocalDateTime());
    FSDK_PDFDictionary_SetAtDateTime(annot_dict, "M", GetLocalDateTime());
    rectf.left = 0;
    rectf.bottom = 650;
    rectf.right = 100;
    rectf.top = 750;
    FSDK_PDFDictionary_SetAtRect(annot_dict, "Rect", rectf);

    FSDK_PDFArray_Create(&coords_array);
    FSDK_PDFArray_AddFloat(coords_array, 20);
    FSDK_PDFArray_AddFloat(coords_array, 670);
    FSDK_PDFArray_AddFloat(coords_array, 80);
    FSDK_PDFArray_AddFloat(coords_array, 730);
    FSDK_PDFDictionary_SetAt(annot_dict, "L", coords_array);
    
    FSDK_PDFArray_Create(&line_style);
    FSDK_PDFArray_AddName(line_style, "None");
    FSDK_PDFArray_AddName(line_style, "OpenArrow");
    FSDK_PDFDictionary_SetAt(annot_dict, "LE", line_style);
    
    FSDK_PDFDictionary_Create(&border_dict);
    FSDK_PDFDictionary_SetAtFloat(border_dict, "W", 2.0f);
    FSDK_PDFDictionary_SetAtName(border_dict, "S", "S");
    FSDK_PDFDictionary_SetAtName(border_dict, "Type", "Border");
    FSDK_PDFDictionary_SetAt(annot_dict, "BS", border_dict);
   
    FSDK_PDFArray_Create(&color_array);
    FSDK_PDFArray_AddFloat(color_array, 1);
    FSDK_PDFArray_AddFloat(color_array, 1);
    FSDK_PDFArray_AddFloat(color_array, 0);
    FSDK_PDFDictionary_SetAt(annot_dict, "C", color_array);    
    FSDK_PDFPage_AddAnnot0(page, annot_dict, &annot);

    FSDK_Annot_ResetAppearanceStream(annot, &result_resetap);
    printf("Add a line annotation by lower level API.\r\n");
  }

  {
    // Add line annotation with caption
    FS_PDFDICTIONARY_HANDLE annot_dict = NULL;
    FSRectF rectf;
    FS_PDFARRAY_HANDLE caption_offset = NULL;
    FS_PDFARRAY_HANDLE coords_array = NULL;
    FS_PDFARRAY_HANDLE line_style = NULL;
    FS_PDFDICTIONARY_HANDLE border_dict = NULL;
    FS_PDFARRAY_HANDLE color_array = NULL;
    FS_ANNOT_HANDLE annot = NULL;
    FS_BOOL result_resetap = FALSE;
    FSDK_PDFDictionary_Create(&annot_dict);
    FSDK_PDFDictionary_SetAtName(annot_dict, "Type", "Annot");
    FSDK_PDFDictionary_SetAtName(annot_dict, "Subtype", "Line");
    FSDK_PDFDictionary_SetAtName(annot_dict, "IT", "LineArrow");

    FSDK_PDFDictionary_SetAtInteger(annot_dict, "F", 4);
    FSDK_PDFDictionary_SetAtString(annot_dict, "Contents", L"A common line.");
    FSDK_PDFDictionary_SetAtString(annot_dict, "T", L"Foxit SDK");
    FSDK_PDFDictionary_SetAtString(annot_dict, "Subj", L"Line");
    FSDK_PDFDictionary_SetAtString(annot_dict, "NM", WRandomUID(uid, uid_size));
    FSDK_PDFDictionary_SetAtDateTime(annot_dict, "CreationDate", GetLocalDateTime());
    FSDK_PDFDictionary_SetAtDateTime(annot_dict, "M", GetLocalDateTime());    
    rectf.left = 0;
    rectf.bottom = 650;
    rectf.right = 100;
    rectf.top = 750;
    FSDK_PDFDictionary_SetAtRect(annot_dict, "Rect", rectf);
    FSDK_PDFDictionary_SetAtBoolean(annot_dict, "Cap", TRUE);
    
    FSDK_PDFArray_Create(&caption_offset);
    FSDK_PDFArray_AddFloat(caption_offset, 0);
    FSDK_PDFArray_AddFloat(caption_offset, 5);
    FSDK_PDFDictionary_SetAt(annot_dict, "CO", caption_offset);
    
    FSDK_PDFArray_Create(&coords_array);
    FSDK_PDFArray_AddFloat(coords_array, 40);
    FSDK_PDFArray_AddFloat(coords_array, 670);
    FSDK_PDFArray_AddFloat(coords_array, 100);
    FSDK_PDFArray_AddFloat(coords_array, 730);
    FSDK_PDFDictionary_SetAt(annot_dict, "L", coords_array);
    
    FSDK_PDFArray_Create(&line_style);
    FSDK_PDFArray_AddName(line_style, "Square");
    FSDK_PDFArray_AddName(line_style, "OpenArrow");
    FSDK_PDFDictionary_SetAt(annot_dict, "LE", line_style);
    
    FSDK_PDFDictionary_Create(&border_dict);
    FSDK_PDFDictionary_SetAtFloat(border_dict, "W", 2.0f);
    FSDK_PDFDictionary_SetAtName(border_dict, "S", "S");
    FSDK_PDFDictionary_SetAtName(border_dict, "Type", "Border");
    FSDK_PDFDictionary_SetAt(annot_dict, "BS", border_dict);
    
    FSDK_PDFArray_Create(&color_array);
    FSDK_PDFArray_AddFloat(color_array, 0);
    FSDK_PDFArray_AddFloat(color_array, 1);
    FSDK_PDFArray_AddFloat(color_array, 0);
    FSDK_PDFDictionary_SetAt(annot_dict, "C", color_array);    
    FSDK_PDFPage_AddAnnot0(page, annot_dict, &annot);
    FSDK_Annot_ResetAppearanceStream(annot, &result_resetap);
    printf("Add a line annotation with caption by lower level API.\r\n");
  }

  {
    // Add circle annotation
    FS_PDFDICTIONARY_HANDLE annot_dict = NULL;
    FSRectF rectf;
    FS_PDFARRAY_HANDLE color_array = NULL;
    FS_ANNOT_HANDLE annot = NULL;
    FS_BOOL result_resetap = FALSE;
    FSDK_PDFDictionary_Create(&annot_dict);
    FSDK_PDFDictionary_SetAtName(annot_dict, "Type", "Annot");
    FSDK_PDFDictionary_SetAtName(annot_dict, "Subtype", "Circle");

    FSDK_PDFDictionary_SetAtInteger(annot_dict, "F", 4);
    FSDK_PDFDictionary_SetAtString(annot_dict, "T", L"Foxit SDK");
    FSDK_PDFDictionary_SetAtString(annot_dict, "Subj", L"Circle");
    FSDK_PDFDictionary_SetAtString(annot_dict, "NM", WRandomUID(uid, uid_size));
    FSDK_PDFDictionary_SetAtDateTime(annot_dict, "CreationDate", GetLocalDateTime());
    FSDK_PDFDictionary_SetAtDateTime(annot_dict, "M", GetLocalDateTime());    
    rectf.left = 120;
    rectf.bottom = 650;
    rectf.right = 180;
    rectf.top = 750;
    FSDK_PDFDictionary_SetAtRect(annot_dict, "Rect", rectf);
    
    FSDK_PDFArray_Create(&color_array);
    FSDK_PDFArray_AddFloat(color_array, 0);
    FSDK_PDFArray_AddFloat(color_array, 1);
    FSDK_PDFArray_AddFloat(color_array, 0);
    FSDK_PDFDictionary_SetAt(annot_dict, "C", color_array);    
    FSDK_PDFPage_AddAnnot0(page, annot_dict, &annot);

    FSDK_Annot_ResetAppearanceStream(annot, &result_resetap);
    printf("Add a circle annotation by lower level API.\r\n");
  }

  {
    // Add square annotation
    FS_PDFDICTIONARY_HANDLE annot_dict = NULL;
    FSRectF rectf;
    FS_PDFARRAY_HANDLE color_array = NULL;
    FS_PDFARRAY_HANDLE interior_color_array = NULL;
    FS_ANNOT_HANDLE annot = NULL;
    FS_BOOL result_resetap = FALSE;
    FSDK_PDFDictionary_Create(&annot_dict);
    FSDK_PDFDictionary_SetAtName(annot_dict, "Type", "Annot");
    FSDK_PDFDictionary_SetAtName(annot_dict, "Subtype", "Square");

    FSDK_PDFDictionary_SetAtInteger(annot_dict, "F", 4);
    FSDK_PDFDictionary_SetAtString(annot_dict, "T", L"Foxit SDK");
    FSDK_PDFDictionary_SetAtString(annot_dict, "Subj", L"Square");
    FSDK_PDFDictionary_SetAtString(annot_dict, "NM", WRandomUID(uid, uid_size));
    FSDK_PDFDictionary_SetAtDateTime(annot_dict, "CreationDate", GetLocalDateTime());
    FSDK_PDFDictionary_SetAtDateTime(annot_dict, "M", GetLocalDateTime());    
    rectf.left = 200;
    rectf.bottom = 650;
    rectf.right = 300;
    rectf.top = 750;
    FSDK_PDFDictionary_SetAtRect(annot_dict, "Rect", rectf);
    
    FSDK_PDFArray_Create(&color_array);
    FSDK_PDFArray_AddFloat(color_array, 0);
    FSDK_PDFArray_AddFloat(color_array, 1);
    FSDK_PDFArray_AddFloat(color_array, 0);
    FSDK_PDFDictionary_SetAt(annot_dict, "C", color_array);    
    FSDK_PDFArray_Create(&interior_color_array);
    FSDK_PDFArray_AddFloat(interior_color_array, 1);
    FSDK_PDFArray_AddFloat(interior_color_array, 1);
    FSDK_PDFArray_AddFloat(interior_color_array, 0);
    FSDK_PDFDictionary_SetAt(annot_dict, "IC", interior_color_array);    
    FSDK_PDFPage_AddAnnot0(page, annot_dict, &annot);
    FSDK_Annot_ResetAppearanceStream(annot, &result_resetap);
    printf("Add a square annotation by lower level API.\r\n");
  }
}

void AddIntToByteArray(size_t value, unsigned char* dest_char_array) {
  unsigned char temp_array[4] = {
    (unsigned char)(value & 0xFF),
    (unsigned char)((value >> 8) & 0xFF),
    (unsigned char)((value >> 16) & 0xFF),
    (unsigned char)((value >> 24) & 0xFF)
  };
  memcpy(dest_char_array, temp_array, 4 * sizeof(unsigned char));
}

void AddShortToByteArray(short value, unsigned char* dest_char_array) {
  unsigned char temp_array[2] = {
    (unsigned char)(value & 0xFF),
    (unsigned char)((value >> 8) & 0xFF)
  };
  memcpy(dest_char_array, temp_array, 2 * sizeof(unsigned char));
}

void AddStringToByteArray(const char* str, unsigned char* dest_char_array) {
  memcpy(dest_char_array, str, strlen(str));
}

FS_BOOL SaveSoundToFile(FS_SOUNDANNOT_HANDLE *sound, const wchar_t* file_path)
{
  FS_PDFSTREAM_HANDLE sound_stream = NULL;
  FS_UINT64 stream_size = 0;
  FILE* file;
  size_t riff_size;
  FS_BOOL ret_value = 0;
  int bit = 0;
  int i = 0;
  unsigned char wav_file_header[44];
  FSSampleEncodingFormat encoding_format;
  size_t chunk_size;
  short format = 1;
  int channel_count = 0;
  unsigned char* data;
  float rate;
  int byte_per_sec;
  short block_align;
  if (!sound) return 0;  
#if defined(_WIN32) || defined(_WIN64) || defined(_WIN_STORE)  
  _wfopen_s(&file, file_path, L"wb");

#else
  FILE* file = fopen(String::FromUnicode(file_path), "wb");
#endif
  if (!file) return 0;
  
  FSDK_Sound_GetSoundStream(sound, &sound_stream);

  FSDK_PDFStream_GetDataSize(sound_stream, 0, &stream_size);

  data = (unsigned char*)malloc((size_t)stream_size);
  FSDK_PDFStream_GetData(sound_stream, 0, (size_t)stream_size, (void*)data, &ret_value);  
  FSDK_Sound_GetBits(sound, &bit);
  riff_size = (size_t)stream_size + 36;

  //write file header:
  memset(wav_file_header, 0, 44);
  
  AddStringToByteArray("RIFF", &wav_file_header[i]);
  i += 4;

  AddIntToByteArray(riff_size, &wav_file_header[i]);
  i += 4;

  AddStringToByteArray("WAVEfmt ", &wav_file_header[i]);
  i += 8;

  chunk_size = 16;
  AddIntToByteArray(chunk_size, &wav_file_header[i]);
  i += 4;
  
  AddShortToByteArray(format, &wav_file_header[i]);
  i += 2;

  
  FSDK_Sound_GetChannelCount(sound, &channel_count);
  AddShortToByteArray(channel_count, &wav_file_header[i]);
  i += 2;

  FSDK_Sound_GetSamplingRate(sound, &rate);
  AddIntToByteArray((int)rate, &wav_file_header[i]);
  i += 4;

  byte_per_sec = (int)rate * channel_count * bit / 8;
  AddIntToByteArray((size_t)byte_per_sec, &wav_file_header[i]);
  i += 4;

  block_align = (short)(bit * channel_count / 8);
  AddShortToByteArray(block_align, &wav_file_header[i]);
  i += 2;

  AddShortToByteArray(bit, &wav_file_header[i]);
  i += 2;

  AddStringToByteArray("data", &wav_file_header[i]);
  i += 4;

  AddIntToByteArray((size_t)stream_size, &wav_file_header[i]);
  fwrite(wav_file_header, 44, 1, file);

  //write file data:
  FSDK_Sound_GetSampleEncodingFormat(sound, &encoding_format);
  switch (encoding_format)
  {
  case e_FSSampleEncodingFormatALaw:
    break;
  case e_FSSampleEncodingFormatMuLaw:
    break;
  case e_FSSampleEncodingFormatSigned:
  {
    unsigned char* buffer = (unsigned char*)malloc((size_t)stream_size + 1);
    size_t j = 0, k = 0;
    size_t i;
    for (i = 0; i < (size_t)stream_size; i += 2) {
      unsigned char low = data[j++];
      unsigned char high;
      if (j == (size_t)stream_size) {
        high = 0;
      }
      else {
        high = data[j++];
      }
      buffer[k++] = high;
      buffer[k++] = low;
    }
    fwrite(buffer, sizeof(unsigned char), (size_t)stream_size, file);
    free(buffer);
  }
  break;
  case e_FSSampleEncodingFormatRaw:
  default:
    fwrite(data, sizeof(unsigned char), (size_t)stream_size, file);
    break;
  }

  free(data);
  fclose(file);
  return 1;
}

FS_ANNOT_HANDLE GetAnnotWithType(FS_PDFPAGE_HANDLE pdf_page, FSAnnotType annot_type, const int annot_index_with_type, int *annot_index_with_all)
{
  int ii = (FSAnnotType)-1;
  int i = 0;
  int annot_count = 0;
  int spec_annot_index = -1;
  FS_ANNOT_HANDLE ret_annot = NULL;
  if (pdf_page == NULL || annot_type < (FSAnnotType)-1) return NULL;

  FSDK_PDFPage_GetAnnotCount(pdf_page, &annot_count);  
  for (i = 0; i < annot_count; i++)
  {
    FS_ANNOT_HANDLE annot_i = NULL;
    int cmp_index = -1;
    FSDK_PDFPage_GetAnnot(pdf_page, i, &annot_i);
    FSDK_Annot_GetType(annot_i, &annot_type);
    if ((FSAnnotType)-1 == annot_type)
    {
      cmp_index = i;
    }
    else if (annot_type == annot_type)
    {
      spec_annot_index++;
      cmp_index = spec_annot_index;
    }
    if (cmp_index == annot_index_with_type)
    {
      *annot_index_with_all = i;
      ret_annot = annot_i;
      break;
    }
  }
  return ret_annot;
}

FS_SOUNDANNOT_HANDLE GetSoundAnnot(const wchar_t *input_pdf_path, int page_index)
{
  int annot_index_with_type = 0;
  int annot_index_with_all = 0;
  FS_FDFDOC_HANDLE doc = NULL;
  FS_PDFPAGE_HANDLE pdf_page = NULL;
  FS_ANNOT_HANDLE annot;
  FSAnnotType annot_type;
  FSDK_PDFDoc_Create0(input_pdf_path, &doc);
  FSDK_PDFDoc_Load(doc, NULL);
  FSDK_PDFDoc_GetPage(doc, page_index, &pdf_page);
  if (pdf_page == NULL)
  {
    FSDK_PDFDoc_Release(doc);
    return NULL;
  }

  annot = GetAnnotWithType(pdf_page, e_FSSound, annot_index_with_type, &annot_index_with_all);
  if (annot == NULL)
  {
    FSDK_PDFDoc_Release(doc);
    return NULL;
  }
  FSDK_Annot_GetType(annot, &annot_type);
  if (annot_type != e_FSSound)
  {
    FSDK_PDFDoc_Release(doc);
    return NULL;
  }
  FSDK_PDFDoc_Release(doc);
  return annot;
}

int main(int argc, char *argv[])
{
  int err_ret = 0;
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  // Initialize library
  FSErrorCode error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    return 1;
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%sannotation_input.pdf", input_path);  
  swprintf_s(output_directory, MAX_FILE_PATH, L"%sannotation/", output_path);
  _wmkdir(output_directory);
  {
    // Load a document
    FS_PDFDOC_HANDLE doc;
    FS_BOOL return_result = 0;
    wchar_t new_pdf[MAX_FILE_PATH] = { 0 };
    FS_PDFPAGE_HANDLE page = NULL;
    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;

    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    // Get first page with index 0
    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    AddAnnotations(page);
    // Save PDF file
    swprintf_s(new_pdf, MAX_FILE_PATH, L"%sannotation.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, new_pdf, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    FSDK_PDFPage_Release(page);
    FSDK_PDFDoc_Release(doc);
  }

  {
    // Load a document
    FS_PDFDOC_HANDLE doc = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    wchar_t new_pdf[MAX_FILE_PATH] = { 0 };
    FS_BOOL return_result = 0;
    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;

    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    // Get first page with index 0
    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    AddAnnotsWithLowLevelAPI(page);
    // Save PDF file
    swprintf_s(new_pdf, MAX_FILE_PATH, L"%sannotation_lower_level_API.pdf", output_directory);
    error_code = FSDK_PDFDoc_SaveAs(doc, new_pdf, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    FSDK_PDFPage_Release(page);
    FSDK_PDFDoc_Release(doc);
  }

  //save sound to file:
  {
    wchar_t output_sound_file[MAX_FILE_PATH] = { 0 };
    wchar_t input_sound_pdf_file[MAX_FILE_PATH] = { 0 };
    FS_SOUNDANNOT_HANDLE sound;
    swprintf_s(output_sound_file, MAX_FILE_PATH, L"%ssound.wav", output_directory);

    swprintf_s(input_sound_pdf_file, MAX_FILE_PATH, L"%ssound.pdf", input_path);
    sound = GetSoundAnnot(input_sound_pdf_file, 0);
    if (SaveSoundToFile(sound, output_sound_file))
    {
      wprintf(L"Save sound annotation to file, saved file path: %ws \n", output_sound_file);
    }
  }
  FSDK_Library_Release();
  return err_ret;
}
