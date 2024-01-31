// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to convert PDF files to Office(Word, Excel or PowerPoint) format files.

// Include Foxit PDF SDK header files.
#if defined(_WIN32) || defined(_WIN64)

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fx_stream_c.h"
#include "../../../include/fs_pdf2office_c.h"

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

static FS_BOOL gNeedToPause(void* user_data) {
  return TRUE;
}

static void gProgressNotify(void* user_data, int converted_count, int total_count) {
}

FS_BOOL is_large_file = FALSE;
FILE* file = NULL;
FILE* filestream = NULL;
int ref = 0;
FS_INT64 cur_pos = 0;

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

FS_INT64 gGetSizeFileStream(void* user_data) {
  if (!user_data) return 0;
  fseek(filestream, 0, SEEK_END);
  return (FS_INT64)ftell(filestream);
}

FS_BOOL gFlushFileStream(void* user_data) {
  fflush(filestream);
  return TRUE;
}

FS_INT64 gGetPositionFileStream(void* user_data) {
  return cur_pos;
}

FS_BOOL gReadBlockFileStream(void* user_data, void* buffer, FS_INT64 offset, size_t size) {
  if (0 != fseek(filestream, (long)offset, 0))
    return FALSE;
  if (0 == fread(buffer, size, sizeof(char), filestream))
    return FALSE;

  cur_pos = offset + size;
  return TRUE;
}

size_t gReadBlock0FileStream(void* user_data, void* buffer, size_t size) {
  if (gReadBlock(user_data, buffer, gGetSize(user_data), size))
    return size;
  else
    return 0;
}

FS_BOOL gWriteBlockFileStream(void* user_data, const void* buffer, FS_INT64 offset, size_t size) {
  FS_INT64 write_size = 0;
  fseek(filestream, (long)offset, SEEK_SET);
  write_size = fwrite(buffer, sizeof(char), size, filestream);
  if (write_size == size) {
    cur_pos = offset + size;
    return TRUE;
  }
  return FALSE;
}

FSIFX_FileStream* gRetainFileStream(void* user_data) {
  ref++;
  return ((FSStreamCallback*)user_data);
}

FS_BOOL gIsEOFFileStream(void* user_data) {
  fseek(filestream, 0, SEEK_END);
  if (cur_pos < (FS_INT64)ftell(filestream))
    return FALSE;
  return TRUE;
}

void gReleaseFileStream(void* user_data) {
  ref--;
  if (ref > 0) return;
  
  fclose(filestream);
  free(user_data);
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t src_pdf_path[MAX_FILE_PATH];
  wchar_t saved_word_file_path[MAX_FILE_PATH];
  wchar_t saved_excel_file_path[MAX_FILE_PATH];
  wchar_t saved_ppt_file_path[MAX_FILE_PATH];
  wchar_t saved_word_file_path_stream[MAX_FILE_PATH];
  wchar_t saved_excel_file_path_stream[MAX_FILE_PATH];
  wchar_t saved_ppt_file_path_stream[MAX_FILE_PATH];

  FSPDF2OfficeSettingData setting_data;
  FSConvertCallback* callback = NULL;
  FS_PROGRESSIVE_HANDLE progressive_handle = NULL;
  int rate = 0;
  FSConvertCallback* callback_stream_word = NULL;
  FSReaderCallback* callback_filereader_word = NULL;
  FSStreamCallback* callback_filestream_word = NULL;
  FSConvertCallback* callback_stream_excel = NULL;
  FSReaderCallback* callback_filereader_excel = NULL;
  FSStreamCallback* callback_filestream_excel = NULL;
  FSConvertCallback* callback_stream_ppt = NULL;
  FSReaderCallback* callback_filereader_ppt = NULL;
  FSStreamCallback* callback_filestream_ppt = NULL;
    
  const wchar_t* metrics_data_folder_path = L"";//Please ensure the path is valid.

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdf2office/", output_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  FSDK_PDF2Office_Initialize(L""); //Please ensure the path is valid.

  // Convert PDF file to Word format file.
  setting_data.metrics_data_folder_path.str = (wchar_t*)metrics_data_folder_path; 
  setting_data.metrics_data_folder_path.len = wcslen(metrics_data_folder_path);
  setting_data.enable_ml_recognition = FALSE;
  setting_data.page_range = NULL;
  setting_data.include_pdf_comments = TRUE;
  setting_data.word_setting_data.enable_retain_page_layout = FALSE;
  

  callback = (FSConvertCallback*)malloc(sizeof(FSConvertCallback));
  callback->user_data = callback;
  callback->NeedToPause = gNeedToPause;
  callback->ProgressNotify = gProgressNotify;

  swprintf_s(src_pdf_path, MAX_FILE_PATH, L"%lsword.pdf", input_path);
  swprintf_s(saved_word_file_path, MAX_FILE_PATH, L"%lspdf2word_result.docx", output_directory);
  swprintf_s(saved_word_file_path_stream, MAX_FILE_PATH, L"%lspdf2word_stream_result.docx", output_directory);

  error_code = FSDK_PDF2Office_StartConvertToWord(src_pdf_path, NULL, saved_word_file_path, setting_data, callback, &progressive_handle);
  if (error_code != e_FSErrSuccess) {
    switch (error_code) {
    case e_FSErrNoPDF2OfficeModuleRight:
      printf("[Failed] Conversion module is not contained in current Foxit PDF Conversion SDK keys.\n");
      break;
    default:
      printf("Exception: %d\n", error_code);
        break;
    }
    return 1;
  }    

  FSDK_Progressive_GetRateOfProgress(progressive_handle, &rate);
  if (rate != 100) {
    FSState state = e_FSToBeContinued;
    while (e_FSToBeContinued == state) {
      FSDK_Progressive_Continue(progressive_handle, &state);
    }
  }
  printf("Convert PDF file to Word format file with path.\n");

  callback_stream_word = (FSConvertCallback*)malloc(sizeof(FSConvertCallback));
  callback_stream_word->user_data = callback_stream_word;
  callback_stream_word->NeedToPause = gNeedToPause;
  callback_stream_word->ProgressNotify = gProgressNotify;

  callback_filereader_word = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
  _wfopen_s(&file, src_pdf_path, L"rb");
  callback_filereader_word->user_data = callback_filereader_word;
  callback_filereader_word->ReadBlock = gReadBlock;
  callback_filereader_word->Release = gRelease;
  callback_filereader_word->GetSize = gGetSize;

  callback_filestream_word = (FSStreamCallback*)malloc(sizeof(FSStreamCallback));
  _wfopen_s(&filestream, saved_word_file_path_stream, L"wb");
  callback_filestream_word->Flush = gFlushFileStream;
  callback_filestream_word->GetPosition = gGetPositionFileStream;
  callback_filestream_word->GetSize = gGetSizeFileStream;
  callback_filestream_word->IsEOF = gIsEOFFileStream;
  callback_filestream_word->WriteBlock = gWriteBlockFileStream;
  callback_filestream_word->ReadBlock = gReadBlockFileStream;
  callback_filestream_word->ReadBlock0 = gReadBlock0FileStream;
  callback_filestream_word->Release = gReleaseFileStream;
  callback_filestream_word->Retain = gRetainFileStream;
  callback_filestream_word->user_data = callback_filestream_word;
  ref = 0;
  cur_pos = 0;

  error_code = FSDK_PDF2Office_StartConvertToWord0(callback_filereader_word, NULL, callback_filestream_word, setting_data, callback_stream_word, &progressive_handle);
  if (error_code != e_FSErrSuccess) {
    switch (error_code) {
    case e_FSErrNoPDF2OfficeModuleRight:
      printf("[Failed] Conversion module is not contained in current Foxit PDF Conversion SDK keys.\n");
      break;
    default:
      printf("Exception: %d\n", error_code);
      break;
    }
    return 1;
  }    
  FSDK_Progressive_GetRateOfProgress(progressive_handle, &rate);
  if (rate != 100) {
    FSState state = e_FSToBeContinued;
    while (e_FSToBeContinued == state) {
      FSDK_Progressive_Continue(progressive_handle, &state);
    }
  }
  printf("Convert PDF file to Word format file with stream.\n");

  swprintf_s(src_pdf_path, MAX_FILE_PATH, L"%lsexcel.pdf", input_path);
  swprintf_s(saved_excel_file_path, MAX_FILE_PATH, L"%lspdf2excel_result.xlsx", output_directory);
  swprintf_s(saved_excel_file_path_stream, MAX_FILE_PATH, L"%lspdf2excel_stream_result.xlsx", output_directory);

  // Convert PDF file to Excel format file.
  error_code = FSDK_PDF2Office_StartConvertToExcel(src_pdf_path, NULL, saved_excel_file_path, setting_data, callback_stream_word, &progressive_handle);
  if (error_code != e_FSErrSuccess) {
    switch (error_code) {
    case e_FSErrNoPDF2OfficeModuleRight:
      printf("[Failed] Conversion module is not contained in current Foxit PDF Conversion SDK keys.\n");
      break;
    default:
      printf("Exception: %d\n", error_code);
      break;
    }
    return 1;
  }    
  FSDK_Progressive_GetRateOfProgress(progressive_handle, &rate);
  if (rate != 100) {
    FSState state = e_FSToBeContinued;
    while (e_FSToBeContinued == state) {
      FSDK_Progressive_Continue(progressive_handle, &state);
    }
  }
  printf("Convert PDF file to Excel format file with path.\n");

  callback_stream_excel = (FSConvertCallback*)malloc(sizeof(FSConvertCallback));
  callback_stream_excel->user_data = callback_stream_word;
  callback_stream_excel->NeedToPause = gNeedToPause;
  callback_stream_excel->ProgressNotify = gProgressNotify;

  callback_filereader_excel = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
  _wfopen_s(&file, src_pdf_path, L"rb");
  callback_filereader_excel->user_data = callback_filereader_word;
  callback_filereader_excel->ReadBlock = gReadBlock;
  callback_filereader_excel->Release = gRelease;
  callback_filereader_excel->GetSize = gGetSize;

  callback_filestream_excel = (FSStreamCallback*)malloc(sizeof(FSStreamCallback));
  _wfopen_s(&filestream, saved_excel_file_path_stream, L"wb");
  callback_filestream_excel->Flush = gFlushFileStream;
  callback_filestream_excel->GetPosition = gGetPositionFileStream;
  callback_filestream_excel->GetSize = gGetSizeFileStream;
  callback_filestream_excel->IsEOF = gIsEOFFileStream;
  callback_filestream_excel->ReadBlock = gReadBlockFileStream;
  callback_filestream_excel->ReadBlock0 = gReadBlock0FileStream;
  callback_filestream_excel->WriteBlock = gWriteBlockFileStream;
  callback_filestream_excel->Release = gReleaseFileStream;
  callback_filestream_excel->Retain = gRetainFileStream;
  callback_filestream_excel->user_data = callback_filestream_excel;
  ref = 0;
  cur_pos = 0;

  error_code = FSDK_PDF2Office_StartConvertToExcel0(callback_filereader_excel, NULL, callback_filestream_excel, setting_data, callback_stream_excel, &progressive_handle);
  if (error_code != e_FSErrSuccess) {
    switch (error_code) {
      case e_FSErrNoPDF2OfficeModuleRight:
        printf("[Failed] Conversion module is not contained in current Foxit PDF Conversion SDK keys.\n");
        break;
      default:
        printf("Exception: %d\n", error_code);
        break;
    }
    return 1;
  }    
  FSDK_Progressive_GetRateOfProgress(progressive_handle, &rate);
  if (rate != 100) {
    FSState state = e_FSToBeContinued;
    while (e_FSToBeContinued == state) {
      FSDK_Progressive_Continue(progressive_handle, &state);
    }
  }
  printf("Convert PDF file to Excel format file with stream.\n");

  // Convert PDF file to PowerPoint format file.
  swprintf_s(src_pdf_path, MAX_FILE_PATH, L"%lspowerpoint.pdf", input_path);
  swprintf_s(saved_ppt_file_path, MAX_FILE_PATH, L"%lspdf2powerpoint_result.pptx", output_directory);
  swprintf_s(saved_ppt_file_path_stream, MAX_FILE_PATH, L"%lspdf2powerpoint_stream_result.pptx", output_directory);

  error_code = FSDK_PDF2Office_StartConvertToPowerPoint(src_pdf_path, NULL, saved_ppt_file_path, setting_data, callback, &progressive_handle);
  if (error_code != e_FSErrSuccess) {
    switch (error_code) {
    case e_FSErrNoPDF2OfficeModuleRight:
      printf("[Failed] Conversion module is not contained in current Foxit PDF Conversion SDK keys.\n");
      break;
    default:
      printf("Exception: %d\n", error_code);
      break;
    }
    return 1;
  }    
  FSDK_Progressive_GetRateOfProgress(progressive_handle, &rate);
  if (rate != 100) {
    FSState state = e_FSToBeContinued;
    while (e_FSToBeContinued == state) {
      FSDK_Progressive_Continue(progressive_handle, &state);
    }
  }
  printf("Convert PDF file to PowerPoint format file with path.\n");

  callback_stream_ppt = (FSConvertCallback*)malloc(sizeof(FSConvertCallback));
  callback_stream_ppt->user_data = callback_stream_word;
  callback_stream_ppt->NeedToPause = gNeedToPause;
  callback_stream_ppt->ProgressNotify = gProgressNotify;

  callback_filereader_ppt = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
  _wfopen_s(&file, src_pdf_path, L"rb");
  callback_filereader_ppt->user_data = callback_filereader_word;
  callback_filereader_ppt->ReadBlock = gReadBlock;
  callback_filereader_ppt->Release = gRelease;
  callback_filereader_ppt->GetSize = gGetSize;

  callback_filestream_ppt = (FSStreamCallback*)malloc(sizeof(FSStreamCallback));
  _wfopen_s(&filestream, saved_ppt_file_path_stream, L"wb");
  callback_filestream_ppt->Flush = gFlushFileStream;
  callback_filestream_ppt->GetPosition = gGetPositionFileStream;
  callback_filestream_ppt->GetSize = gGetSizeFileStream;
  callback_filestream_ppt->IsEOF = gIsEOFFileStream;
  callback_filestream_ppt->ReadBlock = gReadBlockFileStream;
  callback_filestream_ppt->ReadBlock0 = gReadBlock0FileStream;
  callback_filestream_ppt->WriteBlock = gWriteBlockFileStream;
  callback_filestream_ppt->Release = gReleaseFileStream;
  callback_filestream_ppt->Retain = gRetainFileStream;
  callback_filestream_ppt->user_data = callback_filestream_ppt;

  error_code = FSDK_PDF2Office_StartConvertToPowerPoint0(callback_filereader_ppt, NULL, callback_filestream_ppt, setting_data, callback_stream_ppt, &progressive_handle);
  if (error_code != e_FSErrSuccess) {
    switch (error_code) {
    case e_FSErrNoPDF2OfficeModuleRight:
      printf("[Failed] Conversion module is not contained in current Foxit PDF Conversion SDK keys.\n");
      break;
    default:
      printf("Exception: %d\n", error_code);
      break;
    }
    return 1;
  }    
  FSDK_Progressive_GetRateOfProgress(progressive_handle, &rate);
  if (rate != 100) {
    FSState state = e_FSToBeContinued;
    while (e_FSToBeContinued == state) {
      FSDK_Progressive_Continue(progressive_handle, &state);
    }
  }
  printf("Convert PDF file to PowerPoint format file with stream.\n");

  FSDK_PDF2Office_Release();
  FSDK_Library_Release();
  return err_ret;
}
#endif  // #if defined(_WIN32) || defined(_WIN64)

