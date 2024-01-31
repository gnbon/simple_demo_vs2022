// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to convert url web pages or html files to PDF document.
// If using a stream to load the html file, set variable isfilestreamload to true.

// Include Foxit SDK header files.

#include<string.h>
#include<direct.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <math.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_convert_c.h"

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

#define HTML2PDFMSG       printf("Please make sure the key %s is valid and it has value.\n", argv_key); \
                          printf("Usage: html2pdf_xxx <-html <The url or html path>> <-o <output pdf path>> ...\nPlease try 'html2pdf_xxx --help' for more information.\n");

FS_BOOL AnalysisParameter(int argc, char* argv[], FSHTML2PDFSettingData* pdf_setting_data, FS_WSTR* headerfooter_title, FS_WSTR* headerfooter_url, 
  FS_WSTR* bookmark_root_name, FS_WSTR* url_or_html,
  FS_WSTR* engine_path, FS_WSTR* cookies_path, FS_WSTR* output, int* timeout) {
  FS_BSTR bstr;
  int i = 0;

  for (i = 1; i < argc; i = i + 2) {
    char* argv_key = argv[i];
    char*  argv_value;
    if (argc <= i + 1) {
      HTML2PDFMSG
      return FALSE;
    }
    argv_value = argv[i + 1];

    bstr.str = argv_value;
    bstr.len = strlen(argv_value);
    if (strcmp(argv_key, "-w") == 0) pdf_setting_data->page_width = (float)atof(argv_value);
    else if (strcmp(argv_key, "-h") == 0) pdf_setting_data->page_height = (float)atof(argv_value);
    else if (strcmp(argv_key, "-r") == 0) (FSRotation)sscanf(argv_value, "%d", &pdf_setting_data->rotate_degrees);
    else if (strcmp(argv_key, "-ml") == 0) pdf_setting_data->page_margin.left = (float)atof(argv_value);
    else if (strcmp(argv_key, "-mr") == 0) pdf_setting_data->page_margin.right = (float)atof(argv_value);
    else if (strcmp(argv_key, "-mt") == 0) pdf_setting_data->page_margin.top = (float)atof(argv_value);
    else if (strcmp(argv_key, "-mb") == 0) pdf_setting_data->page_margin.bottom = (float)atof(argv_value);
    else if (strcmp(argv_key, "-scale") == 0) (FSHTML2PDFScalingMode)sscanf(argv_value, "%d", &pdf_setting_data->scaling_mode);
    else if (strcmp(argv_key, "-link") == 0) pdf_setting_data->is_convert_link = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-tag") == 0) pdf_setting_data->is_generate_tag = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-bookmarks") == 0) pdf_setting_data->to_generate_bookmarks = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-mode") == 0) (FSHTML2PDFPageMode)sscanf(argv_value, "%d", &pdf_setting_data->page_mode);
    else if (strcmp(argv_key, "-encoding") == 0) (FSHTML2PDFEncodingFormat)sscanf(argv_value, "%d", &pdf_setting_data->encoding_format);
    else if (strcmp(argv_key, "-render_images") == 0) pdf_setting_data->to_render_images = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-remove_underline_for_link") == 0) pdf_setting_data->to_remove_underline_for_link = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-headerfooter") == 0) pdf_setting_data->to_set_headerfooter = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-headerfooter_title") == 0) {
      FSDK_BStr_UTF8Decode(&bstr, headerfooter_title);
    }
    else if (strcmp(argv_key, "-headerfooter_url") == 0) {
      FSDK_BStr_UTF8Decode(&bstr, headerfooter_url);
    }
    else if (strcmp(argv_key, "-bookmark_root_name") == 0) {
      FSDK_BStr_UTF8Decode(&bstr, bookmark_root_name);
    }
    else if (strcmp(argv_key, "-resize_objects") == 0) pdf_setting_data->to_resize_objects = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-print_background") == 0) pdf_setting_data->to_print_background = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-optimize_tag") == 0) pdf_setting_data->to_optimize_tag_tree = (argv_value == "yes") ? TRUE : FALSE;
    else if (strcmp(argv_key, "-media") == 0) (FSHTML2PDFMediaStyle)sscanf(argv_value, "%d", &pdf_setting_data->media_style);
    else if (strcmp(argv_key, "-engine") == 0) FSDK_BStr_UTF8Decode(&bstr, engine_path);
    else if (strcmp(argv_key, "-cookies") == 0) FSDK_BStr_UTF8Decode(&bstr, cookies_path);
    else if (strcmp(argv_key, "-o") == 0) FSDK_BStr_UTF8Decode(&bstr, output);
    else if (strcmp(argv_key, "-html") == 0) FSDK_BStr_UTF8Decode(&bstr, url_or_html);
    else if (strcmp(argv_key, "-timeout") == 0) sscanf(argv_value, "%d", &timeout);
    else {
      HTML2PDFMSG
      return FALSE;
    }
  }
  return TRUE;
}
FS_BOOL is_large_file = FALSE;
FILE* file = NULL;

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

FILE* filepng = NULL;

FS_INT64 gGetSizePng(void* user_data) {
  if (!user_data) return 0;
  if (is_large_file) {
    long long size_long;
    _fseeki64(filepng, 0, SEEK_END);
    size_long = _ftelli64(filepng);
    return size_long;
  } else {
   fseek(filepng, 0, SEEK_END);
   return (FS_INT64)ftell(filepng);
  }
  return 0;
}

FS_BOOL gReadBlockPng(void* user_data, void* buffer, FS_INT64 offset, size_t size) {
  if (is_large_file) {
    long long read_size;
    _fseeki64(filepng, offset, SEEK_SET);

    read_size = fread(buffer, 1, size, filepng);
    return read_size == size ? TRUE : FALSE;
  } else {
    if (!filepng)
      return FALSE;
    if (0 != fseek(filepng, (long)offset, 0))
      return FALSE;
    if (0 == fread(buffer, size, 1, filepng))
      return FALSE;
    return TRUE;
  }
  return FALSE;
}

static void gReleasePng(void* user_data) {
  if (filepng) {
    fclose(filepng);
    filepng = NULL;
  }  
  if (user_data)
   free(user_data);
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

int main(int argc, char *argv[]) {
  FS_BOOL isfilestreamload = FALSE;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t url_or_html[MAX_FILE_PATH];
  // "engine_path" is the path of the engine file "fxhtml2pdf" which is used to convert html to pdf. Please refer to Developer Guide for more details.
  const wchar_t* engine_path = L"";
  // "cookies_path" is the path of the cookies file exported from the web pages that you want to convert. Please refer to Developer Guide for more details.
  const wchar_t* cookies_path = L"";

  wchar_t output_file[MAX_FILE_PATH];
  int time_out = 15;

  FSErrorCode error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  if (argc > 1 && strcmp(argv[1], "--help") == 0) {
    printf("Usage:\nhtml2pdf_xxx <-html <The url or html path>> <-o <output pdf path>> <-engine <htmltopdf engine path>>\n"
          " [-w <page width>] [-h <page height>] [-ml <margin left>] [-mr <margin right>] [-mt <margin top>] [-mb <margin bottom>] [-r <page rotate degree>] [-mode <page mode>] [-scale <whether scale page>]\n"
          " [-link <whether convert link>] [-tag <whether generate tag>] [-bookmarks <whether to generate bookmarks>] [-cookies <cookies file path>] [-timeout <timeout>]\n\n"
          "-html The url or html file path. for examples '-html www.foxitsoftware.com'\n"
          "-o The output pdf path.\n"
          "-engine The html to pdf engine path.\n"
          "-w The page width.\n"
          "-h The page height.\n"
          "-r The page roate degree. '0' means 0 degree, '1' means 90 degree, '2' means 180 degree, '3' means 270 degree.\n"
          "-ml The page margin distance of left.\n"
          "-mr The page margin distance of right.\n"
          "-mt The page margin distance of top.\n"
          "-mb The page margin distance of bottom.\n"
          "-mode The page mode. 0 means single page mode and 1 means multiple mode\n"
          "-scale The scaling mode. '0' means no scale, '1' means scale page, '2' means enlarge page.\n"
          "-link Whether to covert link. 'yes' means to convert link, 'no' means no need to covert.\n"
          "-tag Whether to generate tag. 'yes' means to generate tag, 'no' means no need to generate.\n"
          "-bookmarks Whether to generate bookmarks.'yes' means to generate bookmarks, 'no' means no need to generate.\n"
          "-encoding The HTML encoding format. '0' means auto encoding and '1'-'73' means other encodings\n"
          "-render_images Whether to render images or not.'yes' means to render images, 'no' means no need to render.\n"
          "-remove_underline_for_link Whether to remove underline for link. 'yes' means to remove underline for link, 'no' means no need to remove.\n"
          "-headerfooter Whether to generate headerfooter. 'yes' means to generate headerfooter, 'no' means no need to generate.\n"
          "-headerfooter_title The headerfooter title.\n"
          "-headerfooter_url The headerfooter url.\n"
          "-bookmark_root_name The bookmark root name.\n"
          "-resize_objects Whether to enable the Javascripts related resizing of the objects during rendering process. 'yes' means to enable, 'no' means to disable.\n"
          "-print_background Whether to print background. 'yes' means to print background, 'no' means no need to print background.\n"
          "-optimize_tag Whether to optimize tag tree. 'yes' means to optimize tag tree, 'no' means no need to optimize tag tree.\n"
          "-media The media style. '0' means screen media style, '1' means print media style.\n"
          "-cookies The cookies file path.\n"
          "-timeout The timeout of loading webpages.\n");
    return 0;
  }

  //If using a stream to load the html file, set isfilestreamload to TRUE
  swprintf_s(output_directory, MAX_FILE_PATH, L"%lshtml2pdf/", output_path);
  _wmkdir(output_directory);

  swprintf_s(url_or_html, MAX_FILE_PATH, L"%lsAboutFoxit.html", input_path);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lshtml2pdf_result.pdf", output_directory);

  {
    FSHTML2PDFSettingData pdf_setting_data;
    FS_WSTR headerfooter_title; 
    FS_WSTR headerfooter_url;
    FS_WSTR bookmark_root_name;
    FS_WSTR cookiesstr;
    FS_WSTR url;
    FS_WSTR engine;
    FS_WSTR output;

    pdf_setting_data.is_convert_link = TRUE;
    pdf_setting_data.is_generate_tag = TRUE;
    pdf_setting_data.to_generate_bookmarks = TRUE;
    pdf_setting_data.rotate_degrees = e_FSRotation0;
    pdf_setting_data.page_height = 640;
    pdf_setting_data.page_width = 900;
    pdf_setting_data.page_margin.left = 0;
    pdf_setting_data.page_margin.right = 0;
    pdf_setting_data.page_margin.top = 0;
    pdf_setting_data.page_margin.bottom = 0;
    pdf_setting_data.page_mode = e_FSPageModeSinglePage;
    pdf_setting_data.scaling_mode = e_FSScalingModeScale;
    pdf_setting_data.encoding_format = e_FSEncodingFormatDefault;
    pdf_setting_data.headerfooter_title.str = L"";
    pdf_setting_data.headerfooter_title.len = 0;
    pdf_setting_data.headerfooter_url.str = L"";
    pdf_setting_data.headerfooter_url.len = 0;
    pdf_setting_data.bookmark_root_name.str = L"";
    pdf_setting_data.bookmark_root_name.len = 0;
    pdf_setting_data.to_resize_objects = FALSE;
    pdf_setting_data.to_print_background = TRUE;
    pdf_setting_data.to_optimize_tag_tree = FALSE;
    pdf_setting_data.media_style = e_FSMediaStyleScreen;
    pdf_setting_data.to_load_active_content = FALSE;

    FSDK_WStr_Init(&headerfooter_title);
    FSDK_WStr_Init(&headerfooter_url);
    FSDK_WStr_Init(&bookmark_root_name);
    FSDK_WStr_Init(&cookiesstr);
    FSDK_WStr_Init(&url);
    FSDK_WStr_Init(&engine);
    FSDK_WStr_Init(&output);
    if (!AnalysisParameter(argc, argv, &pdf_setting_data, &headerfooter_title, &headerfooter_url, &bookmark_root_name, &url, &engine, &cookiesstr, &output, &time_out)) return 1;
    if (headerfooter_title.len > 0) {
      pdf_setting_data.headerfooter_title.str = headerfooter_title.str;
      pdf_setting_data.headerfooter_title.len = headerfooter_title.len;
    }
    if (headerfooter_url.len > 0) {
      pdf_setting_data.headerfooter_url.str = headerfooter_url.str;
      pdf_setting_data.headerfooter_url.len = headerfooter_url.len;
    }
    if (bookmark_root_name.len > 0) {
      pdf_setting_data.bookmark_root_name.str = bookmark_root_name.str;
      pdf_setting_data.bookmark_root_name.len = bookmark_root_name.len;
    }
    if (cookiesstr.len > 0) {
      cookies_path = cookiesstr.str;
    }
    if (url.len > 0) {
      swprintf(url_or_html, MAX_FILE_PATH, L"%ls", url.str);
    }
    if (engine.len > 0) {
      engine_path = engine.str;
    }
    if (output.len > 0) {
      swprintf(output_file, MAX_FILE_PATH, L"%ls", output.str);
    }
    FSDK_Convert_FromHTML(url_or_html, engine_path, cookies_path, pdf_setting_data, output_file, time_out);

    printf("Convert HTML to PDF successfully.\n");
  }

  //get HTML data from stream
  if (isfilestreamload) {
    // "htmlfile" is the path of the html file to be loaded. For example: "C:/aaa.html". The method of "FromHTML" will load this file as a stream.
    const wchar_t* htmlfile = L"";
    // "htmlfilepng" is the path of the png resource file to be loaded. For example: "C:/aaa.png". set it in the related_resource_file of HTML2PDFRelatedResource.
    const wchar_t* htmlfilepng = L"";
    //"relativefilepath" is the resource file's relative path. For example: "./aaa.png".
    const char* relativefilepath = "";

    size_t converted_chars = 0;
    FSHTML2PDFSettingData pdf_setting_data;
    const wchar_t* cookies = L"";
    FSRectF rectf;
    FSHTML2PDFRelatedResource html2PDFRelatedResource[1];
    char* curlocale = "";
    const char* source = "";
    size_t char_num = 0;
    wchar_t* desthtmlfilepng = L"";

    FSWriterCallback* filewritercallback = (FSWriterCallback*)malloc(sizeof(FSWriterCallback));
    FSReaderCallback* filereadercallback = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));
    FSReaderCallback* filereadercallbackpng = (FSReaderCallback*)malloc(sizeof(FSReaderCallback));

    pdf_setting_data.page_height = 650;
    pdf_setting_data.page_width = 950;
    pdf_setting_data.is_to_page_scale = FALSE;

    rectf.left = 18;
    rectf.bottom = 18;
    rectf.right = 18;
    rectf.top = 18;
    pdf_setting_data.page_margin = rectf;
    pdf_setting_data.is_convert_link = TRUE;
    pdf_setting_data.rotate_degrees = e_FSRotation0;
    pdf_setting_data.is_generate_tag = TRUE;
    pdf_setting_data.page_mode = e_FSPageModeSinglePage;
    pdf_setting_data.to_generate_bookmarks = TRUE;
    pdf_setting_data.scaling_mode = e_FSScalingModeNone;
    pdf_setting_data.encoding_format = e_FSEncodingFormatDefault;
    pdf_setting_data.to_render_images = TRUE;
    pdf_setting_data.to_remove_underline_for_link = FALSE;
    pdf_setting_data.to_set_headerfooter = FALSE;
    pdf_setting_data.headerfooter_title.str = L"";
    pdf_setting_data.headerfooter_title.len = 0;
    pdf_setting_data.headerfooter_url.str = L"";
    pdf_setting_data.bookmark_root_name.str = L"abcde";
    pdf_setting_data.bookmark_root_name.len = wcslen(L"abcde");
    pdf_setting_data.to_resize_objects = TRUE;
    pdf_setting_data.to_print_background = FALSE;
    pdf_setting_data.to_optimize_tag_tree = FALSE;
    pdf_setting_data.media_style =e_FSMediaStyleScreen;
    pdf_setting_data.to_load_active_content = FALSE;
    
    swprintf_s(output_file, MAX_FILE_PATH, L"%lshtml2pdf_filestream_result.pdf", output_directory);
    
    _wfopen_s(&file_writer, output_file, L"w+b");
    filewritercallback->user_data = filewritercallback;
    filewritercallback->Flush = gFlushWriter;
    filewritercallback->GetSize = gGetSizeWriter;
    filewritercallback->WriteBlock = gWriteBlockWriter;
    filewritercallback->Release = gReleaseWriter;

    _wfopen_s(&file, htmlfile, L"rb");
    filereadercallback->user_data = filereadercallback;
    filereadercallback->GetSize = gGetSize;
    filereadercallback->ReadBlock = gReadBlock;
    filereadercallback->Release = gRelease;


    filereadercallbackpng->user_data = filereadercallbackpng;
    filereadercallbackpng->GetSize = gGetSizePng;
    filereadercallbackpng->ReadBlock = gReadBlockPng;
    filereadercallbackpng->Release = gReleasePng;
    _wfopen_s(&filepng, htmlfilepng, L"rb");
    html2PDFRelatedResource[0].related_resource_file = (FSReaderCallback*)filereadercallbackpng;

    curlocale = setlocale(LC_ALL, NULL);  
    setlocale(LC_ALL, "chs");
    source = relativefilepath;
    char_num = sizeof(char)* strlen(relativefilepath) + 1;
    desthtmlfilepng = malloc(sizeof(wchar_t) * char_num);
    mbstowcs_s(&converted_chars, desthtmlfilepng, char_num, source, _TRUNCATE);

    html2PDFRelatedResource[0].resource_file_relative_path.str =desthtmlfilepng;
    html2PDFRelatedResource[0].resource_file_relative_path.len = wcslen(desthtmlfilepng);
    FSDK_Convert_FromHTML2(filereadercallback, html2PDFRelatedResource, 1,engine_path, NULL,
      pdf_setting_data, filewritercallback, 15);
    free(desthtmlfilepng);
    printf("Convert HTML to PDF successfully by filestream.\n");
  }
  FSDK_Library_Release();
  return 0;
}
