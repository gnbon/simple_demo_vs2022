// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to convert images to PDF files.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>
#include <windows.h>

#define MAX_FILE_PATH 260
#define TRUE 1
#define FALSE 0

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_image_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_convert_c.h"

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

FILE* fileread = NULL;
FILE* filewriter = NULL;

static FS_INT64 gGetSizeFileReader(void* user_data) {
  if (!user_data || !fileread) return 0;
  fseek(fileread, 0, SEEK_END);
  return (FS_INT64)ftell(fileread);
}

static FS_BOOL gReadBlockFileReader(void* user_data, void* buffer, FS_INT64 offset, size_t size) {
  if (!user_data || !fileread) return FALSE;
  if (!fileread)
    return FALSE;
  if (0 != fseek(fileread, (long)(offset), 0))
    return FALSE;
  if (0 == fread(buffer, size, 1, fileread))
    return FALSE;
  return TRUE;
}

static void gReleaseFileReader(void* user_data) {
  if (!user_data || !fileread) return;

  fclose(fileread);
  fileread = NULL;
  if (user_data) free(user_data);
}

FS_INT64 gGetSizeFileWriter(void* user_data) {
  if (!user_data || !filewriter) return FALSE;
  fseek(filewriter, 0, SEEK_END);
  return (FS_UINT32)ftell(filewriter);
}

FS_BOOL gFlushFileWriter(void* user_data) {
  if (!user_data || !filewriter) return FALSE;
  fflush(filewriter);
  return TRUE;
}

FS_BOOL gWriteBlockFileWriter(void* user_data, const void* buffer, FS_INT64 offset, size_t size) {
  FS_UINT64 write_size = 0;
  if (!user_data || !filewriter) return FALSE;

  fseek(filewriter, (long)offset, SEEK_SET);
  write_size = fwrite(buffer, sizeof(char), size, filewriter);
  if (write_size == size) {
    return TRUE;
  }

  return FALSE;
}

static void gReleaseFileWriter(void* user_data) {
  if (filewriter) fclose(filewriter);
  filewriter = NULL;
  if (user_data)
    free(user_data);
}

void Image2PDF(const wchar_t* input_file, const wchar_t* output_file) {
  FS_IMAGE_HANDLE image = NULL;
  FS_BOOL result;
  FSImageType type;
  int count = 1, i = 0;
  FS_PDFDOC_HANDLE doc = NULL;

  FSDK_Image_Create0(input_file, &image);
  FSDK_Image_GetType(image, &type);
  if (type != e_FSJBIG2) {
    FSDK_Image_GetFrameCount(image, &count);
  } 

  FSDK_PDFDoc_Create(&doc);
  for (i = 0; i < count; i++) {
    FS_PDFPAGE_HANDLE page = NULL;
    int width, height;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FSPointF pointf;
    FS_BOOL return_result;
    FSDK_Image_GetWidth(image, &width);
    FSDK_Image_GetHeight(image, &height);
    FSDK_PDFDoc_InsertPage(doc, i, (float)width, (float)height, &page);

    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);
    // Add image to page
    pointf.x = 0;
    pointf.y = 0;

    FSDK_PDFPage_AddImage(page, image, i, pointf, (float)width, (float)height, TRUE, &return_result);
    FSDK_PDFPage_Release(page);
  }
  FSDK_Image_Release(image);
  FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &result);
  FSDK_PDFDoc_Release(doc);
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  FSErrorCode error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsimage2pdf/", output_path);
  _wmkdir(output_directory);

  {
    {
      // Convert .bmp file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lswatermark.bmp", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lswatermark_bmp.pdf", output_directory);
      Image2PDF(input_file,output_file);
      printf("Convert BMP file to PDF file.\n");
    }
    {
      // Convert .jpg file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsimage_samples.jpg", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsimage_samples_jpg.pdf", output_directory);
      Image2PDF(input_file,output_file);
      printf("Convert JPG file to PDF file.\n");
    }
    {
      // Convert .tif file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsTIF2Pages.tif", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsTIF2Pages_tif.pdf", output_directory);
      Image2PDF(input_file,output_file);
      printf("Convert TIF file to PDF file.\n");
    }
    {
      // Convert .gif file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsimage005.gif", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsimage005_gif.pdf", output_directory);
      Image2PDF(input_file,output_file);
      printf("Convert GIF file to PDF file.\n");
    }
    {
      // Convert .png file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.png", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_png.pdf", output_directory);
      Image2PDF(input_file,output_file);
      printf("Convert PNG file to PDF file.\n");
    }
    {
      // Convert .jb2 file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsSTR_039.jb2", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsSTR_039_jb2.pdf", output_directory);
      Image2PDF(input_file,output_file);
      printf("Convert JB2 file to PDF file.\n");
    }
    {
      // Convert .jp2 file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.jp2", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_jp2.pdf", output_directory);
      Image2PDF(input_file,output_file);
      printf("Convert JP2 file to PDF file.\n");
    }
    {
      // Convert .jpx file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.jpx", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_jpx.pdf", output_directory);
      Image2PDF(input_file,output_file);
      printf("Convert JPX file to PDF file.\n");
    }
   // Convert from filepath
    printf("Start: convert file from file path.\n");
    {
      // Convert .bmp file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lswatermark.bmp", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lswatermark_bmp_frompath.pdf", output_directory);
      FSDK_Convert_FromImage(input_file, output_file);
      printf("Convert BMP file to PDF file.\n");
    }
    {
      // Convert .jpg file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsimage_samples.jpg", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsimage_samples_jpg_frompath.pdf", output_directory);

      FSDK_Convert_FromImage(input_file, output_file);
      printf("Convert JPG file to PDF file.\n");
    }
    {
      // Convert .tif file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsTIF2Pages.tif", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsTIF2Pages_tif_frompath.pdf", output_directory);

      FSDK_Convert_FromImage(input_file, output_file);
      printf("Convert TIF file to PDF file.\n");
    }
    {
      // Convert .gif file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsimage005.gif", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsimage005_gif_frompath.pdf", output_directory);
      FSDK_Convert_FromImage(input_file, output_file);
      printf("Convert GIF file to PDF file.\n");
    }
    {
      // Convert .png file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.png", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_png_frompath.pdf", output_directory);

      FSDK_Convert_FromImage(input_file, output_file);
      printf("Convert PNG file to PDF file.\n");
    }
    {
      // Convert .jb2 file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsSTR_039.jb2", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsSTR_039_jb2_frompath.pdf", output_directory);

      FSDK_Convert_FromImage(input_file, output_file);
      printf("Convert JB2 file to PDF file.\n");
    }

    {
      // Convert .jp2 file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.jp2", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_jp2_frompath.pdf", output_directory);

      FSDK_Convert_FromImage(input_file, output_file);
      printf("Convert JP2 file to PDF file.\n");
    }
    {
      // Convert .jpx file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.jpx", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_jpx_frompath.pdf", output_directory);

      FSDK_Convert_FromImage(input_file, output_file);
      printf("Convert JPX file to PDF file.\n");
    }
    printf("End: convert file from file path.\n");

  // Convert from stream
    printf("Start: convert file from stream.\n");
   {
     // Convert .bmp file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
      FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
      swprintf_s(input_file, MAX_FILE_PATH, L"%lswatermark.bmp", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lswatermark_bmp_stream.pdf", output_directory);

      filereadercallback->user_data = filereadercallback;
      filereadercallback->GetSize = gGetSizeFileReader;
      filereadercallback->ReadBlock = gReadBlockFileReader;
      filereadercallback->Release = gReleaseFileReader;
      filewritercallback->user_data = filewritercallback;
      filewritercallback->Flush = gFlushFileWriter;
      filewritercallback->GetSize = gGetSizeFileWriter;
      filewritercallback->WriteBlock = gWriteBlockFileWriter;
      filewritercallback->Release = gReleaseFileWriter;
      _wfopen_s(&fileread, input_file, L"rb");
      _wfopen_s(&filewriter, output_file, L"wb");
      FSDK_Convert_FromImage0(filereadercallback, filewritercallback);
      printf("Convert BMP file to PDF file.\n");
    }
    {
      // Convert .jpg file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
      FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsimage_samples.jpg", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsimage_samples_jpg_stream.pdf", output_directory);

      filereadercallback->user_data = filereadercallback;
      filereadercallback->GetSize = gGetSizeFileReader;
      filereadercallback->ReadBlock = gReadBlockFileReader;
      filereadercallback->Release = gReleaseFileReader;
      filewritercallback->user_data = filewritercallback;
      filewritercallback->Flush = gFlushFileWriter;
      filewritercallback->GetSize = gGetSizeFileWriter;
      filewritercallback->WriteBlock = gWriteBlockFileWriter;
      filewritercallback->Release = gReleaseFileWriter;
      _wfopen_s(&fileread, input_file, L"rb");
      _wfopen_s(&filewriter, output_file, L"wb");
      FSDK_Convert_FromImage0(filereadercallback, filewritercallback);
      printf("Convert JPG file to PDF file.\n");
    }
    {
      // Convert .tif file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
      FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsTIF2Pages.tif", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsTIF2Pages_tif_stream.pdf", output_directory);

      filereadercallback->user_data = filereadercallback;
      filereadercallback->GetSize = gGetSizeFileReader;
      filereadercallback->ReadBlock = gReadBlockFileReader;
      filereadercallback->Release = gReleaseFileReader;
      filewritercallback->user_data = filewritercallback;
      filewritercallback->Flush = gFlushFileWriter;
      filewritercallback->GetSize = gGetSizeFileWriter;
      filewritercallback->WriteBlock = gWriteBlockFileWriter;
      filewritercallback->Release = gReleaseFileWriter;
      _wfopen_s(&fileread, input_file, L"rb");
      _wfopen_s(&filewriter, output_file, L"wb");
      FSDK_Convert_FromImage0(filereadercallback, filewritercallback);
      printf("Convert TIF file to PDF file.\n");
    }
    {
      // Convert .gif file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
      FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsimage005.gif", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsimage005_gif_stream.pdf", output_directory);

      filereadercallback->user_data = filereadercallback;
      filereadercallback->GetSize = gGetSizeFileReader;
      filereadercallback->ReadBlock = gReadBlockFileReader;
      filereadercallback->Release = gReleaseFileReader;
      filewritercallback->user_data = filewritercallback;
      filewritercallback->Flush = gFlushFileWriter;
      filewritercallback->GetSize = gGetSizeFileWriter;
      filewritercallback->WriteBlock = gWriteBlockFileWriter;
      filewritercallback->Release = gReleaseFileWriter;
      _wfopen_s(&fileread, input_file, L"rb");
      _wfopen_s(&filewriter, output_file, L"wb");
      FSDK_Convert_FromImage0(filereadercallback, filewritercallback);
      printf("Convert GIF file to PDF file.\n");
    }
    {
      // Convert .png file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
      FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.png", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_png_stream.pdf", output_directory);

      filereadercallback->user_data = filereadercallback;
      filereadercallback->GetSize = gGetSizeFileReader;
      filereadercallback->ReadBlock = gReadBlockFileReader;
      filereadercallback->Release = gReleaseFileReader;
      filewritercallback->user_data = filewritercallback;
      filewritercallback->Flush = gFlushFileWriter;
      filewritercallback->GetSize = gGetSizeFileWriter;
      filewritercallback->WriteBlock = gWriteBlockFileWriter;
      filewritercallback->Release = gReleaseFileWriter;
      _wfopen_s(&fileread, input_file, L"rb");
      _wfopen_s(&filewriter, output_file, L"wb");
      FSDK_Convert_FromImage0(filereadercallback, filewritercallback);
      printf("Convert PNG file to PDF file.\n");
    }
    {
      // Convert .jb2 file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
      FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
      swprintf_s(input_file, MAX_FILE_PATH, L"%lsSTR_039.jb2", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lsSTR_039_jb2_stream.pdf", output_directory);

      filereadercallback->user_data = filereadercallback;
      filereadercallback->GetSize = gGetSizeFileReader;
      filereadercallback->ReadBlock = gReadBlockFileReader;
      filereadercallback->Release = gReleaseFileReader;
      filewritercallback->user_data = filewritercallback;
      filewritercallback->Flush = gFlushFileWriter;
      filewritercallback->GetSize = gGetSizeFileWriter;
      filewritercallback->WriteBlock = gWriteBlockFileWriter;
      filewritercallback->Release = gReleaseFileWriter;
      _wfopen_s(&fileread, input_file, L"rb");
      _wfopen_s(&filewriter, output_file, L"wb");
      FSDK_Convert_FromImage0(filereadercallback, filewritercallback);

      printf("Convert JB2 file to PDF file.\n");
    }

    {
      // Convert .jp2 file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
      FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.jp2", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_jp2_stream.pdf", output_directory);

      filereadercallback->user_data = filereadercallback;
      filereadercallback->GetSize = gGetSizeFileReader;
      filereadercallback->ReadBlock = gReadBlockFileReader;
      filereadercallback->Release = gReleaseFileReader;
      filewritercallback->user_data = filewritercallback;
      filewritercallback->Flush = gFlushFileWriter;
      filewritercallback->GetSize = gGetSizeFileWriter;
      filewritercallback->WriteBlock = gWriteBlockFileWriter;
      filewritercallback->Release = gReleaseFileWriter;
      _wfopen_s(&fileread, input_file, L"rb");
      _wfopen_s(&filewriter, output_file, L"wb");
      FSDK_Convert_FromImage0(filereadercallback, filewritercallback);

      printf("Convert JP2 file to PDF file.\n");
    }
    {
      // Convert .jpx file to PDF document.
      wchar_t input_file[MAX_FILE_PATH] = { 0 };
      wchar_t output_file[MAX_FILE_PATH] = { 0 };
      FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
      FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));

      swprintf_s(input_file, MAX_FILE_PATH, L"%ls1.jpx", input_path);
      swprintf_s(output_file, MAX_FILE_PATH, L"%ls1_jpx_stream.pdf", output_directory);

      filereadercallback->user_data = filereadercallback;
      filereadercallback->GetSize = gGetSizeFileReader;
      filereadercallback->ReadBlock = gReadBlockFileReader;
      filereadercallback->Release = gReleaseFileReader;
      filewritercallback->user_data = filewritercallback;
      filewritercallback->Flush = gFlushFileWriter;
      filewritercallback->GetSize = gGetSizeFileWriter;
      filewritercallback->WriteBlock = gWriteBlockFileWriter;
      filewritercallback->Release = gReleaseFileWriter;
      _wfopen_s(&fileread, input_file, L"rb");
      _wfopen_s(&filewriter, output_file, L"wb");
      FSDK_Convert_FromImage0(filereadercallback, filewritercallback);

      printf("Convert JPX file to PDF file.\n");
    }
    printf("End: convert file from stream.\n");
  }

  FSDK_Library_Release();
  return err_ret;
}
