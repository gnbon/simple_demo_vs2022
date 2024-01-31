// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to tag a PDF document.

// Include Foxit SDK header files.
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
#include<direct.h>
#else
#include <sys/stat.h>
#endif

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_search_c.h"
#include "../../../include/fs_convert_c.h"
#include "../../../include/fs_taggedpdf_c.h"


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


wchar_t* GetReportCategoryString(wchar_t* format, int format_size, FSReportCategory type) {
  if (format == NULL) return format;
  switch (type) {
  case e_FSReportCategoryRegion:
    swprintf_s(format, format_size, L"Region");
    break;
  case e_FSReportCategoryArtifact:
    swprintf_s(format, format_size, L"Artifact");
    break;
  case e_FSReportCategoryParagraph:
    swprintf_s(format, format_size, L"Paragraph");

    break;
  case e_FSReportCategoryListItem:
    swprintf_s(format, format_size, L"List Item");
    break;
  case e_FSReportCategoryFigure:
    swprintf_s(format, format_size, L"Figure");
    break;
  case e_FSReportCategoryTable:
    swprintf_s(format, format_size, L"Table");
    break;
  case e_FSReportCategoryTableRow:
    swprintf_s(format, format_size, L"Table Row");
    break;
  case e_FSReportCategoryTableHeader:
    swprintf_s(format, format_size, L"Table Header");
    break;
  case e_FSReportCategoryTocItem:
    swprintf_s(format, format_size, L"Toc Item");
    break;
  default:
    swprintf_s(format, format_size, L"");
    break;
  }
  return format;
}

wchar_t* GetReportConfidenceString(wchar_t* format, int format_size, FSReportConfidence type) {
  if (format == NULL) return format;
  switch (type) {
  case e_FSReportConfidenceHigh:
    swprintf_s(format, format_size, L"High");
    break;
  case e_FSReportConfidenceMediumHigh:
    swprintf_s(format, format_size, L"Medium High");
    break;
  case e_FSReportConfidenceMedium:
    swprintf_s(format, format_size, L"Medium");
    break;
  case e_FSReportConfidenceMediumLow:
    swprintf_s(format, format_size, L"Medium Low");
    break;
  case e_FSReportConfidenceLow:
    swprintf_s(format, format_size, L"Low");
    break;
  default:
    swprintf_s(format, format_size, L"");
    break;
  }
  return format;
}

void gRelease(void* user_data) {
  free(user_data);
}

void gReport(void* user_data, FSReportCategory category, FSReportConfidence confidence, int page_index, const FSRectF rect) {
  wchar_t wstr_category[100] = { 0 };
  wchar_t wstr_confidence[100] = { 0 };
  wprintf(L"Page Index: %d, ReportCategory: %s, ReportConfidence: %s, Rect: [%f,%f,%f,%f]\n",
    page_index, GetReportCategoryString(wstr_category, 100, category),
    GetReportConfidenceString(wstr_confidence, 100, confidence), rect.left, rect.top, rect.right, rect.bottom);
}

int main(int argc, char *argv[])
{
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  FSErrorCode error_code;
  swprintf_s(output_directory, MAX_FILE_PATH, L"%staggedpdf/", output_path);
  _wmkdir(output_directory);
  swprintf_s(input_file, MAX_FILE_PATH, L"%sAboutFoxit.pdf", input_path);

  // Initialize library
  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }
  {
    wchar_t output_file[MAX_FILE_PATH] = { 0 };
    FS_PDFDOC_HANDLE pdf_doc = NULL;
    FS_TAGGEDPDF_HANDLE tagged_pdf;
    FS_PROGRESSIVE_HANDLE progressive;
    FSState progressive_state;
    FS_BOOL return_value = FALSE;
    swprintf_s(output_file, MAX_FILE_PATH, L"%sTaggedPdf_StartAutoTagged.pdf", output_directory);

    FSDK_PDFDoc_Create0(input_file, &pdf_doc);
    FSDK_PDFDoc_Load(pdf_doc, NULL);    
    error_code = FSDK_TaggedPDF_Create(pdf_doc, &tagged_pdf);    
    FSDK_TaggedPDF_StartTagDocument(tagged_pdf, NULL, &progressive);
    progressive_state = e_FSToBeContinued;
    while (progressive_state == e_FSToBeContinued)
      FSDK_Progressive_Continue(progressive, &progressive_state);
    error_code = FSDK_PDFDoc_SaveAs(pdf_doc, output_file, 0, &return_value);
    FSDK_TaggedPDF_Release(tagged_pdf);
    FSDK_PDFDoc_Release(pdf_doc);
    if (error_code != e_FSErrSuccess) {
      return 1;
    }
  }
  {//Use callback.
    wchar_t output_file[MAX_FILE_PATH] = { 0 };
    FS_PDFDOC_HANDLE pdf_doc = NULL;
    FS_TAGGEDPDF_HANDLE tagged_pdf = NULL;
    FSTaggedPDFCallback* callback;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FSState progressive_state;
    FS_BOOL return_value = FALSE;
    swprintf_s(output_file, MAX_FILE_PATH, L"%sTaggedPdf_StartAutoTagged_SetCallback.pdf", output_directory);
    FSDK_PDFDoc_Create0(input_file, &pdf_doc);
    FSDK_PDFDoc_Load(pdf_doc, NULL);    
    error_code = FSDK_TaggedPDF_Create(pdf_doc, &tagged_pdf);
    callback = (FSTaggedPDFCallback*)malloc(sizeof(FSTaggedPDFCallback));
    callback->user_data = (void*)callback;
    callback->Release = gRelease;
    callback->Report = gReport;
    FSDK_TaggedPDF_SetCallback(tagged_pdf, callback);    
    FSDK_TaggedPDF_StartTagDocument(tagged_pdf, NULL, &progressive);
    progressive_state = e_FSToBeContinued;
    while (progressive_state == e_FSToBeContinued)
      FSDK_Progressive_Continue(progressive, &progressive_state);    
    error_code = FSDK_PDFDoc_SaveAs(pdf_doc, output_file, 0, &return_value);
    FSDK_TaggedPDF_Release(tagged_pdf);
    FSDK_PDFDoc_Release(pdf_doc);
    if (error_code != e_FSErrSuccess) {
      return 1;
    }
  }
  return 0;
}

