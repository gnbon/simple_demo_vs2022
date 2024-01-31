// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to get/set attributes of viewer preference
// and values of metadata in a PDF file.

// Include Foxit SDK header files.
#include <time.h>
#include<stdarg.h>
#include<direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_pdfdocviewerprefs_c.h"
#include "../../../include/fs_pdfmetadata_c.h"
#include "../../../include/fs_pdfobject_c.h"

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

wchar_t output_directory[MAX_FILE_PATH];

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

void Write(FILE* file, const wchar_t* format, ...) {
  va_list vars;
  if (file) {
    // Get variable list
    va_start(vars, format);
    // Call vfwprintf to format log data and output to log file
    vfwprintf(file, format, vars);
    // End variable list
    va_end(vars);
  }
}

void WriteByteString(FILE* file, const char* format, ...) {
  va_list vars;
  if (file) {
    // Get variable list
    va_start(vars, format);
    // Call vfprintf to format log data and output to log file
    vfprintf(file, format, vars);
    // End variable list
    va_end(vars);
  }
}

FSViewerPref kUIItems[] = {
  e_FSHideToolbar, e_FSHideMenubar,
  e_FSHideWindowUI, e_FSFitWindow,
  e_FSCenterWindow, e_FSDisplayDocTitle };

const FSDisplayMode kDisplayModes[] = { e_FSDisplayUseNone,   e_FSDisplayUseOutlines,
e_FSDisplayUseThumbs, e_FSDisplayFullScreen,
e_FSDisplayUseOC,     e_FSDisplayUseAttachment };

const char* kDisplayModeStrings[] = { "UseNone", "UseOutlines", "UseThumbs", "FullScreen", "UseOC", "UseAttachment" };

const char* kBoxItemStrings[] = { "ViewArea", "ViewClip", "PrintArea", "PrintClip" };
const char* GetPageBoxName(FSBoxType type) {
  switch (type) {
  case e_FSMediaBox:
    return "MediaBox";
  case e_FSCropBox:
    return "CropBox";
  case e_FSTrimBox:
    return "TrimBox";
  case e_FSArtBox:
    return "ArtBox";
  case e_FSBleedBox:
    return "BleedBox";
  }
  return "";
}

const char* GetViewerPrefName(FSViewerPref ui_item) {
  switch (ui_item) {
  case e_FSHideToolbar:
    return "HideToolbar";
  case e_FSHideMenubar:
    return "HideMenubar";
  case e_FSHideWindowUI:
    return "HideWindowUI";
  case e_FSFitWindow:
    return "FitWindow";
  case e_FSCenterWindow:
    return "CenterWindow";
  case e_FSDisplayDocTitle:
    return "DisplayDocTitle";
  }
  return "";
}

const char* GetViewerPrefPrintScaleName(FSPrintScale ui_item) {
  switch (ui_item) {
  case e_FSPrintScaleNone:
    return "PrintScaleNone";
  case e_FSPrintScaleAppDefault:
    return "PrintScaleAppDefault";
  }
  return "";
}

void ShowDocViewerPrefsInfo(FS_PDFDOC_HANDLE doc, const wchar_t* file_name) {
  wchar_t file_info[MAX_FILE_PATH];
  FILE* file = NULL;
  FS_DOCVIEWERPREFS_HANDLE prefs = NULL;
  unsigned int i = 0;
  FSDisplayMode mode;
  const char* display_mode = NULL;
  FS_BOOL result = FALSE;
  FSBoxType type;
  FSPrintScale print_scale;
  int copies;
  FS_RANGE_HANDLE print_ranges;
  FS_BOOL is_empty;

  swprintf_s(file_info, MAX_FILE_PATH, L"%lsDocViewerPrefsInfo_%ls", output_directory, file_name);

  _wfopen_s(&file, file_info, L"w");
  FSDK_DocViewerPrefs_Create(doc, NULL, &prefs);
  WriteByteString(file, "Document viewer preferences information:\r\n");
  // Get UI visibility status.
  for (i = 0; i < sizeof(kUIItems) / (sizeof(kUIItems[0])); i++) {
    FS_BOOL return_result;
    FSDK_DocViewerPrefs_GetUIDisplayStatus(prefs, kUIItems[i], &return_result);
    WriteByteString(file, "Visibility of %s:\t%s\r\n", GetViewerPrefName(kUIItems[i]), return_result ? "Yes" : "No");
  }
  // Get display mode for non full-screen mode.
  FSDK_DocViewerPrefs_GetNonFullScreenPageMode(prefs, &mode);

  for (i = 0; i < sizeof(kDisplayModes) / sizeof(kDisplayModes[i]); i++) {
    if (mode == kDisplayModes[i]) {
      display_mode = kDisplayModeStrings[i];
      break;
    }
  }
  WriteByteString(file, "None full screen page mode:\t%s\r\n", display_mode);
  // Get reading direction.

  FSDK_DocViewerPrefs_GetReadingDirection(prefs, &result);
  WriteByteString(file, "Reading direction:\t%s\r\n", result ? "left to right" : "right to left");
  // Get the type of area item.
  FSDK_DocViewerPrefs_GetPrintClip(prefs, &type);
  WriteByteString(file, "The GetPrintClip returned:\t%s\r\n", (const char*)GetPageBoxName(type));
  FSDK_DocViewerPrefs_GetPrintArea(prefs, &type);
  WriteByteString(file, "The GetPrintArea returned:\t%s\r\n", (const char*)GetPageBoxName(type));
  FSDK_DocViewerPrefs_GetViewArea(prefs, &type);
  WriteByteString(file, "The GetViewArea returned:\t%s\r\n", (const char*)GetPageBoxName(type));
  FSDK_DocViewerPrefs_GetViewClip(prefs, &type);
  WriteByteString(file, "The GetViewClip returned:\t%s\r\n", (const char*)GetPageBoxName(type));

  // Get page scaling option.
  FSDK_DocViewerPrefs_GetPrintScale(prefs, &print_scale);
  WriteByteString(file, "Page scaling option:\t%s\r\n", GetViewerPrefPrintScaleName(print_scale));
  // Get the number of copies to be printed.
  FSDK_DocViewerPrefs_GetPrintCopies(prefs, &copies);
  WriteByteString(file, "The number of copies to be printed:\t%d\r\n", copies);
  // Get page ranges which allowed to print
  WriteByteString(file, "Page ranges for printing:\r\n");

  FSDK_DocViewerPrefs_GetPrintRange(prefs, &print_ranges);
  FSDK_Range_IsEmpty(print_ranges, &is_empty);
  if(!is_empty) {
    int count;
    int j = 0;
    FSDK_Range_GetSegmentCount(print_ranges, &count);

    for (j = 0; j < count; j = j + 1) {
      int start, end;
      FSDK_Range_GetSegmentStart(print_ranges, j, &start);
      FSDK_Range_GetSegmentEnd(print_ranges, j, &end);
      WriteByteString(file, "\tfirst:\t%d\tlast:\t%d\r\n", start, end);
    }
  }
  fclose(file);
}

void SetDocViewerPrefsInfo(FS_PDFDOC_HANDLE doc) {
  unsigned int i = 0;
  FS_DOCVIEWERPREFS_HANDLE prefs = NULL;
  int count;
  FS_RANGE_HANDLE print_range = NULL;

  FSDK_PDFDoc_SetDisplayMode(doc, e_FSDisplayFullScreen);

  FSDK_DocViewerPrefs_Create(doc, NULL, &prefs);
  // Set UI visibility status.
  for (i = 0; i < sizeof(kUIItems) / (sizeof(kUIItems[0])); i++) {
    FSDK_DocViewerPrefs_SetUIDisplayStatus(prefs, kUIItems[i], TRUE);
  }
  // Set display mode for non full-screen mode.
  FSDK_DocViewerPrefs_SetNonFullScreenPageMode(prefs, e_FSDisplayUseOutlines);
  // Set reading direction.
  FSDK_DocViewerPrefs_SetReadingDirection(prefs, FALSE);
  // Set the type of area item.

  FSDK_DocViewerPrefs_SetViewArea(prefs, e_FSCropBox);
  FSDK_DocViewerPrefs_SetViewClip(prefs, e_FSCropBox);
  FSDK_DocViewerPrefs_SetPrintArea(prefs, e_FSCropBox);
  FSDK_DocViewerPrefs_SetPrintClip(prefs, e_FSCropBox);
  // Set page scaling option.
  FSDK_DocViewerPrefs_SetPrintScale(prefs, e_FSPrintScaleNone);
  // Set the number of copies to be printed.
  FSDK_DocViewerPrefs_SetPrintCopies(prefs, 4);

  FSDK_PDFDoc_GetPageCount(doc, &count);
  FSDK_Range_Create1(0, count / 2, e_FSAll, &print_range);
  FSDK_DocViewerPrefs_SetPrintRange(prefs, print_range);
  FSDK_Range_Release(print_range);
  FSDK_DocViewerPrefs_Release(prefs);
}

const wchar_t* kMetadataItems[] = { L"Title", L"Author", L"Subject", L"Keywords",
L"Creator", L"Producer", L"CreationDate", L"ModDate" };

void ShowMetaDataInfo(FS_PDFDOC_HANDLE doc, const wchar_t* file_name) {
  wchar_t output_file[MAX_FILE_PATH];
  FILE* file = NULL;
  FS_METADATA_HANDLE metadata = NULL;
  unsigned int i = 0;

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsMetaDataInfo_%ls", output_directory, file_name);

  _wfopen_s(&file, output_file, L"w");

  WriteByteString(file, "Metadata information:\r\n");

  FSDK_Metadata_Create(doc, &metadata);
  for (i = 0; i < sizeof(kMetadataItems) / sizeof(kMetadataItems[0]); i++) {
    const wchar_t* item = kMetadataItems[i];
    FS_UINT32 count = 0;
	FS_WSTR* value = NULL;
	size_t j = 0;
    Write(file, L"%s: ", item);
    FSDK_Metadata_GetValues(metadata, item, NULL, &count);
    value = (FS_WSTR*)malloc(count * sizeof(FS_WSTR));
    FSDK_Metadata_GetValues(metadata, item, value, &count);
    
    for(j=0;j<count;j++) {
      Write(file, L"%s", value[j].str);
    }
    WriteByteString(file, "\r\n");
    free(value);
  }
  fclose(file);
}

void SetMetaDataInfo(FS_PDFDOC_HANDLE doc) {
  const wchar_t* kMetadataValues[] = { L"Title set by simple demo", L"Simple demo",
    L"Subject set by simple demo", L"Keywords set by simple demo",
    L"Foxit PDF SDK", L"Foxit" };
  FS_METADATA_HANDLE metadata = NULL;
  FS_UINT32 count = sizeof(kMetadataValues) / sizeof(kMetadataValues[0]);
  FS_WSTR* metadata_value = NULL;
  unsigned int i = 0;

  FSDK_Metadata_Create(doc, &metadata);

  metadata_value = (FS_WSTR*)malloc(count * sizeof(FS_WSTR));

  for (i = 0; i < sizeof(kMetadataValues) / sizeof(kMetadataValues[0]); i++) {
    FS_BOOL return_value;
    int length = wcslen(kMetadataValues[i]) * sizeof(wchar_t) + 1;
    FSDK_WStr_Init(&metadata_value[i]);
    FSDK_WStr_SetLength(&metadata_value[i], length);
    FSDK_WStr_Set(&metadata_value[i], kMetadataValues[i], length);

    FSDK_Metadata_SetValues(metadata, kMetadataItems[i], &metadata_value[i], 1, &return_value);
  }
  for (i = 0; i < sizeof(kMetadataValues) / sizeof(kMetadataValues[0]); i++)
    FSDK_WStr_Clear(&metadata_value[i]);
  FSDK_Metadata_SetCreationDateTime(metadata, GetLocalDateTime());
  FSDK_Metadata_SetModifiedDateTime(metadata, GetLocalDateTime());
  free(metadata_value);
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsdocinfo/", output_path);
  _wmkdir(output_directory);

  {
    wchar_t input_file[MAX_FILE_PATH] = { 0 };
    FS_PDFDOC_HANDLE doc = NULL;
	FSErrorCode code = e_FSErrSuccess;
    wchar_t output_file[MAX_FILE_PATH] = { 0 };
	FS_BOOL is_save;

    swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);

    FSDK_PDFDoc_Create0(input_file, &doc);
    code = FSDK_PDFDoc_Load(doc, NULL);
    if (code != e_FSErrSuccess) {
      wprintf(L"[Failed] Cannot load PDF document %ls.\r\nError Message: %d\r\n", input_file, code);
      return 1;
    }
    // Show original information.
    ShowDocViewerPrefsInfo(doc, L"original.txt");
    ShowMetaDataInfo(doc, L"original.txt");

    // Set information.
    SetDocViewerPrefsInfo(doc);
    SetMetaDataInfo(doc);

    // Show new information.
    ShowDocViewerPrefsInfo(doc,L"new.txt");
    ShowMetaDataInfo(doc,L"new.txt");

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsAboutFoxit_docinfo.pdf", output_directory);

    FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &is_save);
    FSDK_PDFDoc_Release(doc);
  }

  FSDK_Library_Release();
  printf("doc info demo done.\n");
  return err_ret;
}
