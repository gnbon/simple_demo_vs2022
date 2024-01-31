// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to extract attached files from PDF files and
// add files as attachments to PDF files.

// Include Foxit SDK header files.
#include <time.h>
#include <stdarg.h>
#include <direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_filespec_c.h"
#include "../../../include/fs_pdfattachments_c.h"
#include "../../../include/fs_pdfnametree_c.h"

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

int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  wchar_t file_info[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;

  FSErrorCode error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsattachment/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsattachment.pdf", input_path);

  swprintf_s(file_info, MAX_FILE_PATH, L"%lsattachMent_info.txt", output_directory);

  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  {
    // Get information of attachments.
    FS_ATTACHMENTS_HANDLE attachments = NULL;
    FS_PDFNAMETREE_HANDLE pdfnametree = NULL;
    FILE* file = NULL;
    int count = 0;
    int i = 0;

    FSDK_PDFNameTree_Create0(&pdfnametree);
    FSDK_Attachments_Create(doc, pdfnametree, &attachments);

    _wfopen_s(&file, file_info, L"w+b");

    FSDK_Attachments_GetCount(attachments, &count);
    for (i = 0; i < count; i++) {
      FS_WSTR key;
      FS_BOOL return_value;
      FS_FILESPEC_HANDLE file_spec = NULL;

      FSDK_WStr_Init(&key);
      FSDK_Attachments_GetKey(attachments, i, &key);

      FSDK_Attachments_GetEmbeddedFile(attachments, (const wchar_t*)key.str, &file_spec);
      Write(file, L"key:\t\t%s\r\n", key.str);
      FSDK_WStr_Clear(&key);

      FSDK_FileSpec_IsEmpty(file_spec, &return_value);
      if (!return_value) {
        FS_WSTR name;
        FS_INT64 size;
        FS_BOOL is_embedded;
        FSDateTime creation_date_time;
        FSDateTime modified_date_time;
        wchar_t return_string[MAX_FILE_PATH];

        FSDK_WStr_Init(&name);
        FSDK_FileSpec_GetFileName(file_spec, &name);
        Write(file, L"Name:\t\t%s\r\n", name.str);
        FSDK_FileSpec_GetFileSize(file_spec, &size);
        Write(file, L"Size:\t\t%d\r\n", size);
        FSDK_FileSpec_IsEmbedded(file_spec, &is_embedded);
        FSDK_FileSpec_GetCreationDateTime(file_spec, &creation_date_time);
        FSDK_FileSpec_GetModifiedDateTime(file_spec, &modified_date_time);
        Write(file, L"Is embedded:\t%ls\r\n", is_embedded ? L"true" : L"false");

        swprintf_s(return_string, MAX_FILE_PATH, L"%d/%d/%d-%d:%d:%d %ls%d:%d", creation_date_time.year, creation_date_time.month, creation_date_time.day, creation_date_time.hour,
        creation_date_time.minute, creation_date_time.second, creation_date_time.utc_hour_offset > 0 ? L"+" : L"-", creation_date_time.utc_hour_offset,
        creation_date_time.utc_minute_offset);

        Write(file, L"Creation time:\t%ls\r\n", return_string);
        swprintf_s(return_string, MAX_FILE_PATH, L"%d/%d/%d-%d:%d:%d %ls%d:%d", modified_date_time.year, modified_date_time.month, modified_date_time.day, modified_date_time.hour,
        modified_date_time.minute, modified_date_time.second, modified_date_time.utc_hour_offset > 0 ? L"+" : L"-", modified_date_time.utc_hour_offset,
        modified_date_time.utc_minute_offset);

        Write(file, L"Modified time:\t%ls\r\n", return_string);
        Write(file, L"\r\n");
        if (is_embedded) {
          wchar_t ex_file_path[MAX_FILE_PATH];
          FS_BOOL is_export_status;

          swprintf_s(ex_file_path, MAX_FILE_PATH, L"%ls%ls", output_directory, name.str );
          FSDK_FileSpec_ExportToFile(file_spec, ex_file_path, &is_export_status);
          wprintf(L"%ls ExportToFile %ls\n", name.str, (is_export_status ? L"successful" : L"failed"));
        }
        FSDK_WStr_Clear(&name);
      }
    }
    fclose(file);
    FSDK_Attachments_Release(attachments);
    FSDK_PDFNameTree_Release(pdfnametree);
  }
  {
    // Remove all attachments
    FS_ATTACHMENTS_HANDLE attachment = NULL;
    FS_PDFNAMETREE_HANDLE pdfnametrees = NULL;
    FS_BOOL result;
    wchar_t new_pdf[MAX_FILE_PATH] = { 0 };
    FS_BOOL return_result = FALSE;

    FSDK_PDFNameTree_Create0(&pdfnametrees);
    FSDK_Attachments_Create(doc, pdfnametrees, &attachment);

    FSDK_Attachments_RemoveAllEmbeddedFiles(attachment, &result);

    swprintf_s(new_pdf, MAX_FILE_PATH, L"%lsattachment_remove.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, new_pdf, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    printf("Remove all attachments.\n");
    FSDK_Attachments_Release(attachment);
    FSDK_PDFNameTree_Release(pdfnametrees);
  }
  {
    wchar_t text_path[MAX_FILE_PATH] = { 0 };
    FS_ATTACHMENTS_HANDLE attachment = NULL;
    FS_PDFNAMETREE_HANDLE pdfnametrees = NULL;
    FS_BOOL result = FALSE;
    wchar_t new_pdf[MAX_FILE_PATH] = { 0 };
    FS_BOOL return_result = FALSE;

    swprintf_s(text_path, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", output_directory);

    FSDK_PDFNameTree_Create0(&pdfnametrees);
    FSDK_Attachments_Create(doc, pdfnametrees, &attachment);
    // Add a new attachment to PDF document.
    FSDK_Attachments_AddFromFilePath(attachment, L"OriginalAttachmentsInfo", text_path, &result);

    swprintf_s(new_pdf, MAX_FILE_PATH, L"%lsattachment_add.pdf", output_directory);

    error_code = FSDK_PDFDoc_SaveAs(doc, new_pdf, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    printf("Add a new attachment to PDF document.\n");
    FSDK_Attachments_Release(attachment);
    FSDK_PDFNameTree_Release(pdfnametrees);
  }

  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();
  return err_ret;
}
