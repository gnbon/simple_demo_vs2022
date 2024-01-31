// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to loading PDF document asynchronously.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fx_basic_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_annot_c.h"
#include "../../../include/fs_render_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
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

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

// Data of asynchronous loader callback object.
typedef struct _DownloadHintDataInfo {
  FS_INT64 offset;
  FS_INT64 size;
  FS_BOOL is_downloaded;
} DownloadHintDataInfo;

FS_BOOL is_large_file = FALSE;
FILE* file = NULL;
DownloadHintDataInfo* hint_data_record = NULL;
int hint_data_record_length = 0;

FS_INT64 gGetSize(void* user_data) {
  if (!user_data) return 0;
  if (is_large_file) {
    long long size_long;
    _fseeki64(file, 0, SEEK_END);
    size_long = _ftelli64(file);
    return size_long;
  } else {
   fseek(file, 0, SEEK_END);
   return (FS_INT64)ftell(file);
  }
  return 0;
}

FS_BOOL gReadBlock(void* user_data, void* buffer, FS_INT64 offset, size_t size) {
  if (is_large_file) {
    long long read_size;
    _fseeki64(file, offset, SEEK_SET);

    read_size = fread(buffer, 1, size, file);
    return read_size == size ? TRUE : FALSE;
  } else {
    if (!file)
      return FALSE;
    if (0 != fseek(file, (long)offset, 0))
      return FALSE;
    if (0 == fread(buffer, size, 1, file))
      return FALSE;
    return TRUE;
  }
  return FALSE;
}

static void gRelease(void* user_data) {
  if (file) {
    fclose(file);
    file = NULL;
  }  
  if (user_data)
   free(user_data);
}

FS_BOOL DownloadData(void* user_data, FS_INT64 offset, FS_INT64 size) {
  DownloadHintDataInfo new_info;
  new_info.is_downloaded = TRUE;
  new_info.offset = offset;
  new_info.size = size;
  hint_data_record_length++;
  hint_data_record = realloc(hint_data_record, sizeof(DownloadHintDataInfo) * hint_data_record_length);
  hint_data_record[hint_data_record_length - 1] = new_info;

  return TRUE;
}

FS_BOOL CheckRecordDownloaded(void* user_data, FS_INT64 offset, FS_INT64 size, FS_BOOL to_download) {
  size_t record_count = hint_data_record_length;
  size_t i = 0;
  for (i = 0; i < record_count; i++) {
    DownloadHintDataInfo data_info = hint_data_record[i];

    // If (offset+size) is out of current data_info, just continue to check other record in hint_data_record_.
    if (offset >(data_info.offset + data_info.size))
      continue;
    if (offset + size < data_info.offset)
      continue;

    // If data defined by <offset, size> has been in/within current data info, just download the data.
    if (offset >= data_info.offset && (offset + size) <= (data_info.offset + data_info.size)) {
      if (to_download)
        data_info.is_downloaded = TRUE;
      return data_info.is_downloaded;
    }

    // If only part of data defined by <offset, size> is in current data_info, download current data_info and
    // also check and download rest data.
    if (offset >= data_info.offset && offset < (data_info.offset + data_info.size) &&
        (offset + size) > (data_info.offset + data_info.size)) {
        FS_INT64 new_offset, new_size = 0;
        if (to_download)
          data_info.is_downloaded = TRUE;
        if (!data_info.is_downloaded)
          return data_info.is_downloaded;
        new_offset = data_info.offset + data_info.size + 1;
        new_size = size - 1 - (data_info.offset + data_info.size - offset);
        return CheckRecordDownloaded(user_data, new_offset, new_size, to_download);
    }

    if (offset < data_info.offset && (offset + size) >= data_info.offset &&
      (offset + size) <= (data_info.offset + data_info.size)) {
      FS_INT64 new_offset, new_size = 0;
      if (to_download)
        data_info.is_downloaded = TRUE;
      if (!data_info.is_downloaded)
        return data_info.is_downloaded;
      new_offset = offset;
      new_size = data_info.offset - 1 - offset;
      return CheckRecordDownloaded(user_data, new_offset, new_size, to_download);
    }
    if (offset < data_info.offset && (offset + size) >(data_info.offset + data_info.size)) {
      FS_INT64 new_offset, new_size = 0;
      if (to_download)
        data_info.is_downloaded = TRUE;
      if (!data_info.is_downloaded)
        return data_info.is_downloaded;
      new_offset = offset;
      new_size = data_info.offset - 1 - offset;
      if (CheckRecordDownloaded(user_data, new_offset, new_size, to_download)) {
        new_offset = data_info.offset + data_info.size + 1;
        new_size = size - 1 - (data_info.offset - offset + data_info.size);
        return CheckRecordDownloaded(user_data, new_offset, new_size, to_download);
      }
      else
        return FALSE;
    }
  }
  if (to_download) {
    return DownloadData(user_data, offset, size);
  }
  return FALSE;
}

// This callback just simulates the downloading progress by DownloadHintDataInfo::is_downloaded,
// not really to download data.
FS_BOOL gIsDataAvail(void* user_data, FS_INT64 offset, size_t size) {
  return CheckRecordDownloaded(user_data, offset, size, FALSE);
}

FS_BOOL gAddDownloadHint(void* user_data, FS_INT64 offset, size_t size) {
  // Record the range and downloaded data.
  return CheckRecordDownloaded(user_data, offset, size, TRUE);
}

int main(int argc, char *argv[]) {
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t input_file[MAX_FILE_PATH];
  wchar_t output_file[MAX_FILE_PATH];
  int err_ret = 0;
  FSAsyncReaderCallback* asyncfile_read = NULL;
  FSReaderCallback* filereadercallback = NULL;
  FS_PDFDOC_HANDLE doc = NULL;
  FS_PDFPAGE_HANDLE page = NULL;
  FS_PROGRESSIVE_HANDLE progressivehandle = NULL;
  float floatwidth, floatheight;
  FSRotation rotate;
  FSMatrix matrix;
  FS_BITMAP_HANDLE bitmap = NULL;
  FS_RENDERER_HANDLE render = NULL;
  FS_IMAGE_HANDLE image = NULL;
  FS_BOOL result = FALSE;
  int width, height = 0;

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsasync_load/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lspage0.bmp", output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }
  filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));

  filereadercallback->user_data = filereadercallback;
  filereadercallback->GetSize = gGetSize;
  filereadercallback->ReadBlock = gReadBlock;
  filereadercallback->Release = gRelease;

  _wfopen_s(&file, input_file, L"rb");

  asyncfile_read = (FSAsyncReaderCallback*)malloc(sizeof(FSAsyncReaderCallback));
  asyncfile_read->readercallback = filereadercallback;
  asyncfile_read->user_data = asyncfile_read;
  asyncfile_read->AddDownloadHint = gAddDownloadHint;
  asyncfile_read->IsDataAvail = gIsDataAvail;

  FSDK_PDFDoc_Create3(asyncfile_read, &doc);

  // Actually, here, application should download needed data which specified by AsyncFileRead::AddDownloadHint().
  // But here, for simple example, we just "download" these data inside AsyncFileRead::AddDownloadHint().
  // So, just continue to check the ready state here, which will trigger AsyncFileRead::AddDownloadHint() to
  // "download" data.
  error_code = e_FSErrDataNotReady;
  while (error_code == e_FSErrDataNotReady)
    error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  // Actually, here, application should download needed data which specified by AsyncFileRead::AddDownloadHint().
  // But here, for simple example, we just "download" these data inside AsyncFileRead::AddDownloadHint().
  // So, just continue to check the ready state here, which will trigger AsyncFileRead::AddDownloadHint() to
  // "download" data.
  error_code = e_FSErrDataNotReady;
  while (error_code == e_FSErrDataNotReady) {
    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
  }
  // Parse page.
  FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, 0, &progressivehandle);
  FSDK_Progressive_Release(progressivehandle);
  progressivehandle = 0;

  FSDK_PDFPage_GetWidth(page, &floatwidth);
  FSDK_PDFPage_GetHeight(page, &floatheight);
  width = (int)(floatwidth);
  height = (int)(floatheight);

  FSDK_PDFPage_GetRotation(page, &rotate);

  matrix.a = 1;
  matrix.b = 0;
  matrix.c = 0;
  matrix.d = 1;
  matrix.e = 0;
  matrix.f = 0;
  FSDK_PDFPage_GetDisplayMatrix(page, 0, 0, width, height, rotate, &matrix);

  // Prepare a bitmap for rendering.
  FSDK_Bitmap_Create(width, height, e_FSDIBArgb, NULL, 0, &bitmap);
  FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, NULL);

  // Render page
  FSDK_Renderer_Create(bitmap, FALSE, &render);

  FSDK_Renderer_StartRender(render, page, matrix, NULL, &progressivehandle);

  FSDK_Image_Create(&image);

  FSDK_Image_AddFrame(image, bitmap, &result);
  FSDK_Image_SaveAs(image, output_file, &result);

  FSDK_Renderer_Release(render);
  FSDK_Bitmap_Release(bitmap);
  FSDK_Image_Release(image);
  FSDK_PDFPage_Release(page);
  FSDK_PDFDoc_Release(doc);

  FSDK_Library_Release();
  free(hint_data_record);
  printf("async-load demo finished.\n");

  return err_ret;
}

