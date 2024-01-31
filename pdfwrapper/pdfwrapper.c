// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to save a PDF document as a wrapper file
// and then open the wrapper file.

#include <time.h>
#include<direct.h>
#include<stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_render_c.h"
#include "../../../include/fs_image_c.h"

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
#define MAX_FILE_BUFFER 1024

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

wchar_t output_directory[MAX_FILE_PATH];

void WriteUnicode(FILE* file, const wchar_t* text_content) {
  FS_WSTR str;
  FS_BSTR dst_bstr;

  str.str = (wchar_t*)text_content;
  str.len = wcslen(text_content);
  FSDK_WStr_UTF16LEEncode(&str, &dst_bstr);

  if (dst_bstr.len == 0) return;
  fwrite((const char*)dst_bstr.str, sizeof(char), dst_bstr.len, file);
}

void RenderPDF2Img(FS_PDFDOC_HANDLE doc) {
  int page_count, i = 0;
  FSDK_PDFDoc_GetPageCount(doc, &page_count);

  for (i = 0; i < page_count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    FS_PROGRESSIVE_HANDLE return_value = NULL;
    float fwidth = 0.0f;
    float fheight = 0.0f;
    int width = 0, height = 0;
    FSMatrix matrix;
    FSRotation rotation;
    FS_BITMAP_HANDLE bitmap = NULL;
    FS_RENDERER_HANDLE renderer = NULL;
    FS_PROGRESSIVE_HANDLE progreessive;
    FS_IMAGE_HANDLE image = NULL;
    wchar_t image_name_new[100];
    wchar_t output_image_name[MAX_FILE_PATH];
    FS_BOOL returncode;
    FS_BOOL returncode2;
    FSErrorCode errorcode = e_FSErrSuccess;
    FSDK_PDFDoc_GetPage(doc, i, &page);

    // Parse page.
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);

    FSDK_PDFPage_GetWidth(page, &fwidth);
    FSDK_PDFPage_GetHeight(page, &fheight);
    width = (int)(fwidth);
    height = (int)(fheight);

    FSDK_PDFPage_GetRotation(page, &rotation);
    FSDK_PDFPage_GetDisplayMatrix(page, 0, 0, width, height, rotation, &matrix);

    // Prepare a bitmap for rendering.
    FSDK_Bitmap_Create(width, height, e_FSDIBArgb, NULL, 0, &bitmap);
    FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, NULL);

    // Render page.
    FSDK_Renderer_Create(bitmap, FALSE, &renderer);

    FSDK_Renderer_StartRender(renderer, page, matrix, NULL, &progreessive);

    // Add the bitmap to an image and save the image.
    FSDK_Image_Create(&image);
    FSDK_Image_AddFrame(image, bitmap, &returncode);

    swprintf_s(image_name_new, 100, L"AboutFoxit_%d", i);
    swprintf_s(output_image_name, MAX_FILE_PATH, L"%lsAboutFoxit_%d_wrapper.jpg", output_directory, i);

    errorcode = FSDK_Image_SaveAs(image, output_image_name, &returncode2);
    if (errorcode != e_FSErrSuccess) {
      printf("[Failed] Cannot save image file.\r\nPage index: %d", i);
    }
    FSDK_Bitmap_Release(bitmap);
    FSDK_Image_Release(image);
    FSDK_PDFPage_Release(page);
  }
}

FILE* file_reader = NULL;
FS_INT64 filesize_reader = 0;
static FS_INT64 gGetSizeReader(void* user_data) {
  return filesize_reader;
}

static FS_BOOL gReadBlockReader(void* user_data, void* buffer, FS_INT64 offset,  size_t size) {
  if (!file_reader) return FALSE;
  if (0 != fseek(file_reader, (long)offset, 0))
    return FALSE;
  return fread(buffer, size, 1, file_reader);
}

static void gReleaseReader(void* user_data) {
  if (file_reader) {
    fclose(file_reader);
    file_reader = NULL;
    filesize_reader = 0;
  }
  if (user_data)
    free (user_data);
}

FILE* file_writer = NULL;

static FS_INT64 gGetSizeWriter(void* user_data) {
  if(!file_writer) return 0;
  fseek(file_writer, 0, SEEK_END);
  return (FS_UINT32)ftell(file_writer);
}

static FS_BOOL gFlushWriter(void* user_data) {
  if(!file_writer) return FALSE;
  fflush(file_writer);
  return TRUE;
}

static FS_BOOL gWriteBlockWriter(void* user_data, const void* buffer, FS_INT64 offset, size_t size) {
  FS_UINT64 write_size = 0;
  if(!file_writer) return 0;

  fseek(file_writer, (long)offset, SEEK_SET);
  write_size = fwrite(buffer, sizeof(char), size, file_writer);
  if(write_size == size) {
    return TRUE;
  }

  return FALSE;
}

static void gReleaseWriter(void* user_data) {
  if (file_writer) {
    fclose(file_writer);
    file_writer = NULL;
  }
  if (user_data)
    free (user_data);
}

FS_BOOL OpenWrapperFile(const wchar_t* file_name) {
  wchar_t file_info[MAX_FILE_PATH];
  FILE* txtfile = NULL;
  FS_PDFDOC_HANDLE doc = NULL;
  FSErrorCode code = e_FSErrSuccess;
  FS_BOOL return_iswrapper = FALSE;
  FS_INT64 offset = 0;
  FSWrapperData wrapper_data;
  FS_PDFDOC_HANDLE doc_real = NULL;
  wchar_t filebuffer[MAX_FILE_BUFFER];
  FSReaderCallback * filereader_callback = NULL;

  swprintf_s(file_info, MAX_FILE_PATH, L"%lsWrapperinfo.txt", output_directory);
  _wfopen_s(&txtfile, file_info, L"w+b");

  code = FSDK_PDFDoc_Create0(file_name, &doc);
  if (code != e_FSErrSuccess) return TRUE;

  code = FSDK_PDFDoc_Load(doc, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf_s(L"[Failed] Cannot load PDF document: %ls\r\nError Message: %d\r\n", file_name, code);
    return FALSE;
  }

  FSDK_PDFDoc_IsWrapper(doc, &return_iswrapper);
  if (!return_iswrapper) {
    wprintf_s(L"[Failed] %ls is not a wrapper file.\r\n", file_name);
    return FALSE;
  }

  FSDK_PDFDoc_GetWrapperOffset(doc, &offset);

  swprintf_s(filebuffer, MAX_FILE_BUFFER, L"offset: %llu\r\n", offset);
  WriteUnicode(txtfile,filebuffer);

  FSDK_PDFDoc_GetWrapperData(doc, &wrapper_data);
  swprintf_s(filebuffer, MAX_FILE_BUFFER, L"version: %d\r\n", wrapper_data.version);
  WriteUnicode(txtfile,filebuffer);
  swprintf_s(filebuffer, MAX_FILE_BUFFER, L"type: %ls\r\n", wrapper_data.type.str);
  WriteUnicode(txtfile,filebuffer);
  swprintf_s(filebuffer, MAX_FILE_BUFFER, L"app_id: %ls\r\n", wrapper_data.app_id.str);
  WriteUnicode(txtfile,filebuffer);
  swprintf_s(filebuffer, MAX_FILE_BUFFER, L"uri: %ls\r\n", wrapper_data.uri.str);
  WriteUnicode(txtfile,filebuffer);
  swprintf_s(filebuffer, MAX_FILE_BUFFER, L"description: %ls\r\n", wrapper_data.description.str);
  WriteUnicode(txtfile,filebuffer);

  //"offset" can also indicate the end position of the document or the document size
  _wfopen_s(&file_reader, file_name, L"rb");
  filesize_reader = offset;
  filereader_callback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
  filereader_callback->user_data = filereader_callback;
  filereader_callback->GetSize = gGetSizeReader;
  filereader_callback->ReadBlock = gReadBlockReader;
  filereader_callback->Release = gReleaseReader;

  code = FSDK_PDFDoc_Create2(filereader_callback, &doc_real);
  if (code != e_FSErrSuccess) return 1;
  code = FSDK_PDFDoc_Load(doc_real, NULL);
  if (code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_real);
    printf("[Failed] Cannot load real PDF document.\r\nError Message: %d\r\n", code);
    return 1;
  }

  RenderPDF2Img(doc_real);
  return TRUE;
}

int main(int argc, char *argv[]) {
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH];
  wchar_t output_file[MAX_FILE_PATH];
  wchar_t input_file2[MAX_FILE_PATH];
  wchar_t input_path_new[MAX_FILE_PATH];
  wchar_t input_path_new2[MAX_FILE_PATH];
  wchar_t out_file_new[MAX_FILE_PATH];
  
  int err_ret = 0;
  FS_PDFDOC_HANDLE doc_wrapper = NULL;
  FSWrapperData wrapperData;
  FS_WSTR type;
  FS_WSTR uri;
  FS_WSTR description;
  FS_BOOL return_result = FALSE;
  FS_PDFDOC_HANDLE doc_real = NULL;
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  FS_PDFDOC_HANDLE doc_rmsv2 = NULL;
  FSWrapperType wrappertype;

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdfwrapper/", output_path);
  swprintf_s(input_file, MAX_FILE_PATH, L"%lswrapper.pdf", input_path);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lsAboutFoxit_wrapper.pdf", output_directory);
  swprintf_s(input_file2, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  // Load PDF document.
  error_code = FSDK_PDFDoc_Create0(input_file, &doc_wrapper);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc_wrapper, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_wrapper);
    wprintf(L"[Failed] Cannot load PDF document: %ls.\r\nError Code: %d\r\n", input_file, error_code);
    return 1;
  }
  // Save the PDF document as a wrapper file.

  wrapperData.version = 10;

  type.str = L"Foxit";
  type.len = wcslen(L"Foxit");
  wrapperData.type = type;
  wrapperData.app_id = type;

  uri.str = L"www.foxitsoftware.com";
  uri.len = wcslen(L"www.foxitsoftware.com");
  wrapperData.uri = uri;

  description.str = L"foxit";
  description.len = wcslen(L"foxit");
  wrapperData.description = description;

  error_code = FSDK_PDFDoc_Create0(input_file2, &doc_real);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc_real, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_real);
    wprintf(L"[Failed] Cannot load PDF document: %ls.\r\nError Code: %d\r\n", input_file2, error_code);
    return 1;
  }

  error_code = FSDK_PDFDoc_SaveAs(doc_real, output_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc_real);
    return 1;
  }

  // "0xFFFFFFFC" can be changed to other values defined in enumeration FSUserPermissions.
  FSDK_PDFDoc_SaveAsWrapperFile(doc_wrapper, output_file, &wrapperData, 0xFFFFFFFC, "", &return_result);

  // Open the wrapper file.
  if (!OpenWrapperFile(output_file)) {
    return 1;
  }

  printf("Add wrapper to PDF file.\n");

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsAboutFoxit_payloadfile.pdf", output_directory);
  swprintf_s(input_path_new, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);

  FSDK_PDFDoc_StartSaveAsPayloadFile(doc_wrapper, output_file, input_path_new, L"Unknown", L"no_description", 1.0, e_FSSaveFlagIncremental, NULL, &progressive);
  printf("Save as payload file.\n");

  swprintf_s(input_path_new2, MAX_FILE_PATH, L"%lsMicrosoft IRM protection 2.pdf", input_path);

  error_code = FSDK_PDFDoc_Create0(input_path_new2, &doc_rmsv2);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc_rmsv2, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc_rmsv2);
    wprintf(L"[Failed] Cannot load PDF document: %ls.\r\nError Code: %d\r\n", input_path_new2, error_code);
    return 1;
  }

  FSDK_PDFDoc_GetWrapperType(doc_rmsv2, &wrappertype);
  if (e_FSWrapperPDFV2 == wrappertype) {
    FS_PROGRESSIVE_HANDLE progressive2 = NULL;
    FSWriterCallback* payloadfile = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
    swprintf_s(out_file_new, MAX_FILE_PATH, L"%lsMicrosoftIRMServices Protected PDF.pdf", output_directory);
    _wfopen_s(&file_writer, out_file_new, L"w+b");
    payloadfile->user_data = payloadfile;
    payloadfile->Flush = gFlushWriter;
    payloadfile->GetSize = gGetSizeWriter;
    payloadfile->Release = gReleaseWriter;
    payloadfile->WriteBlock = gWriteBlockWriter;
    FSDK_PDFDoc_StartGetPayloadFile(doc_rmsv2, payloadfile, NULL, &progressive2);
  }

  printf("Get RMS V2 payload file.\n");
  FSDK_Library_Release();
  return err_ret;
}

