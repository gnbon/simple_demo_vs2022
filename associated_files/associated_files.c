
// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to associate files with PDF.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"

#include "../../../include/fs_common_c.h"
#include "../../../include/fs_filespec_c.h"
#include "../../../include/fs_pdfassociatefiles_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
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

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  wchar_t output_directory[MAX_FILE_PATH] = { 0 };

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAF_Catalog_Page_Annot.pdf", input_path);
  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsassociated_files/", output_path);
  _wmkdir(output_directory);

  // Get information about associated files from PDF document.
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_ASSOCIATEDFILES_HANDLE associated_files = NULL;
    FS_PDFDICTIONARY_HANDLE doc_catalog_dict = NULL;
	int count = 0;
	FS_FILESPEC_HANDLE filespec = NULL;
    FS_WSTR str;
    wchar_t output_file[MAX_FILE_PATH] = { 0 };
    FS_BOOL return_result = FALSE;
	FS_PDFPAGE_HANDLE page = NULL;
    FS_PDFDICTIONARY_HANDLE page_dict = NULL;
    FS_ANNOT_HANDLE annot = NULL;
    FS_PDFDICTIONARY_HANDLE annot_dict = NULL;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) {
      printf("Open file failed: %d\n", error_code);
      return 1;
    }
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      printf("Load document failed: %d\n", error_code);
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    error_code = FSDK_AssociatedFiles_Create(doc, &associated_files);

    error_code = FSDK_PDFDoc_GetCatalog(doc, &doc_catalog_dict);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFDoc_Release(doc);
      return 1;
    }

    error_code = FSDK_AssociatedFiles_GetAssociatedFilesCount(associated_files, doc_catalog_dict, &count);
    printf("The catalog dictionary of \"AF_Catalog_Page_Annot.pdf\" has %d associated files.\n", count);

    error_code = FSDK_AssociatedFiles_GetAssociatedFile(associated_files, doc_catalog_dict, 0, &filespec);

    FSDK_WStr_Init(&str);
    error_code = FSDK_FileSpec_GetFileName(filespec, &str);
    wprintf(L"The file name is \"%ls\".\n\n", str.str);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsaf_1.txt", output_directory);

    FSDK_FileSpec_ExportToFile(filespec, output_file, &return_result);

    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
    if (error_code != e_FSErrSuccess) return 1;

    error_code = FSDK_PDFPage_GetDict(page, &page_dict);
    if (error_code != e_FSErrSuccess) {
      FSDK_PDFPage_Release(page);
      return 1;
    }
    FSDK_AssociatedFiles_GetAssociatedFilesCount(associated_files, page_dict, &count);
    printf("The page dictionary of \"AF_Catalog_Page_Annot.pdf\" has %d associated files.\n", count);
    FSDK_AssociatedFiles_GetAssociatedFile(associated_files, page_dict, 0, &filespec);
    FSDK_FileSpec_GetFileName(filespec, &str);
    wprintf(L"The file name is \"%ls\".\n\n", str.str);
    swprintf_s(output_file, MAX_FILE_PATH, L"%lsaf_2.txt", output_directory);
    FSDK_FileSpec_ExportToFile(filespec, output_file, &return_result);

    FSDK_PDFPage_GetAnnot(page, 0, &annot);

    FSDK_Annot_GetDict(annot, &annot_dict);
    FSDK_AssociatedFiles_GetAssociatedFilesCount(associated_files, annot_dict, &count);
    printf("The annot dictionary of \"AF_Catalog_Page_Annot.pdf\" has %d associated files.\n", count);
    FSDK_AssociatedFiles_GetAssociatedFile(associated_files, annot_dict, 0, &filespec);
    FSDK_FileSpec_GetFileName(filespec, &str);
    wprintf(L"The file name is \"%ls\".\n\n", str.str);
    swprintf_s(output_file, MAX_FILE_PATH, L"%lsaf_4.txt", output_directory);
    FSDK_FileSpec_ExportToFile(filespec, output_file, &return_result);
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAF_ImageXObject_FormXObject.pdf", input_path);
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_BOOL return_result = FALSE;
    FS_ASSOCIATEDFILES_HANDLE associated_files = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FS_POSITION pos = NULL;
    FS_GRAPHICSOBJECT_HANDLE image_x_object = NULL;
    FS_PDFSTREAM_HANDLE pdfstream = NULL;
    FS_PDFDICTIONARY_HANDLE image_x_object_dict = NULL;
    int count = 0;
    FS_WSTR str;
    wchar_t output_file[MAX_FILE_PATH] = { 0 };
    FS_FILESPEC_HANDLE filespec = NULL;
    FS_GRAPHICSOBJECT_HANDLE form_x_object = NULL;
    FS_PDFDICTIONARY_HANDLE form_x_object_dict = NULL;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      printf("Open file failed: %d\n", error_code);
      return 1;
    }

    error_code = FSDK_AssociatedFiles_Create(doc, &associated_files);

    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);

    FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeImage, &pos);

    FSDK_GraphicsObjects_GetGraphicsObject(page, pos, &image_x_object);

    FSDK_ImageObject_GetStream(image_x_object, &pdfstream);
    FSDK_PDFStream_GetDictionary(pdfstream, &image_x_object_dict);

    FSDK_AssociatedFiles_GetAssociatedFilesCount(associated_files, image_x_object_dict, &count);
    printf("The image x object of \"AF_ImageXObject_FormXObject.pdf\" has %d associated files.\n", count);
    FSDK_AssociatedFiles_GetAssociatedFile(associated_files, image_x_object_dict, 0, &filespec);

    FSDK_WStr_Init(&str);
    FSDK_FileSpec_GetFileName(filespec, &str);
    wprintf(L"The file name is \"%ls\".\n\n", str.str);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsaf_5.txt", output_directory);
    FSDK_FileSpec_ExportToFile(filespec, output_file, &return_result);

    FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeFormXObject, &pos);

    FSDK_GraphicsObjects_GetGraphicsObject(page, pos, &form_x_object);

    FSDK_FormXObject_GetStream(form_x_object, &pdfstream);
    FSDK_PDFStream_GetDictionary(pdfstream, &form_x_object_dict);
    FSDK_AssociatedFiles_GetAssociatedFilesCount(associated_files, form_x_object_dict, &count);
    printf("The form x object of \"AF_ImageXObject_FormXObject.pdf\" has %d associated files.\n", count);
    FSDK_AssociatedFiles_GetAssociatedFile(associated_files, form_x_object_dict, 0, &filespec);
    FSDK_FileSpec_GetFileName(filespec, &str);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsaf_6.txt", output_directory);
    wprintf(L"The file name is \"%ls\".\n\n", str.str);
    FSDK_FileSpec_ExportToFile(filespec, output_file, &return_result);
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAF_MarkedContent.pdf", input_path);
  {
    FS_BOOL return_result = FALSE;
    FS_PDFDOC_HANDLE doc = NULL;
    FS_ASSOCIATEDFILES_HANDLE associated_files = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FS_WSTR str;
    FS_POSITION pos = NULL;
    int count = 0;
    FS_GRAPHICSOBJECT_HANDLE text_object = NULL;
    wchar_t output_file[MAX_FILE_PATH] = { 0 };
    FS_FILESPEC_HANDLE filespec = NULL;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      printf("Open file failed: %d\n", error_code);
      return 1;
    }

    error_code = FSDK_AssociatedFiles_Create(doc, &associated_files);
    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);

    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);

    FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeText, &pos);
    FSDK_GraphicsObjects_GetGraphicsObject(page, pos, &text_object);

    FSDK_AssociatedFiles_GetAssociatedFilesCount0(associated_files, text_object, &count);
    printf("The text object of \"AF_MarkedContent.pdf\" has %d associated files.\n", count);
    FSDK_AssociatedFiles_GetAssociatedFile0(associated_files, text_object, 0, &filespec);

    FSDK_WStr_Init(&str);
    FSDK_FileSpec_GetFileName(filespec, &str);
    wprintf(L"The file name is \"%ls\".\n\n", str.str);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lsbitmap.bmp", output_directory);
    FSDK_FileSpec_ExportToFile(filespec, output_file, &return_result);

    FSDK_AssociatedFiles_GetAssociatedFile0(associated_files, text_object, 1, &filespec);
    FSDK_FileSpec_GetFileName(filespec, &str);

    swprintf_s(output_file, MAX_FILE_PATH, L"%lstext.txt", output_directory);
    wprintf(L"The file name is \"%ls\".\n\n", str.str);
    FSDK_FileSpec_ExportToFile(filespec, output_file, &return_result);
  }

  // Associate files with objects in PDF document.
  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAssociateTestFile.pdf", input_path);
  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_ASSOCIATEDFILES_HANDLE associated_files = NULL;
    FS_FILESPEC_HANDLE filespec_txt = NULL;
    FSDateTime dateTime;
    FS_BOOL return_result = FALSE;
    wchar_t output_file[MAX_FILE_PATH] = { 0 };
    FS_PDFPAGE_HANDLE page = NULL;
    FS_PROGRESSIVE_HANDLE progressive = NULL;
    FS_FILESPEC_HANDLE filespec_jpg = NULL;

    error_code = FSDK_PDFDoc_Create0(input_file, &doc);
    if (error_code != e_FSErrSuccess) return 1;
    error_code = FSDK_PDFDoc_Load(doc, NULL);
    if (error_code != e_FSErrSuccess) {
      printf("Open file failed: %d\n", error_code);
      return 1;
    }

    error_code = FSDK_AssociatedFiles_Create(doc, &associated_files);

    //Create filespec with 1.txt
    FSDK_FileSpec_Create(doc, &filespec_txt);
    FSDK_FileSpec_SetAssociteFileRelationship(filespec_txt, e_FSRelationshipSource);
    FSDK_FileSpec_SetFileName(filespec_txt, L"1.txt");

    dateTime.year = 2017;
    dateTime.month = 9;
    dateTime.day = 15;
    dateTime.hour = 17;
    dateTime.minute = 20;
    dateTime.second = 20;
    dateTime.milliseconds = 0;
    dateTime.utc_hour_offset = 8;
    dateTime.utc_minute_offset = 0;
    FSDK_FileSpec_SetDescription(filespec_txt, "text");

    swprintf_s(output_file, MAX_FILE_PATH, L"%ls1.txt", output_directory);
    FSDK_FileSpec_Embed(filespec_txt, output_file, &return_result);
    FSDK_FileSpec_GetCreationDateTime(filespec_txt, &dateTime);
    FSDK_FileSpec_SetModifiedDateTime(filespec_txt, dateTime);
    FSDK_FileSpec_SetSubtype(filespec_txt, "application/octet-stream");

    //Associate 1.txt with catalog dictionary.
    {
      FS_PDFOBJECT_HANDLE catalog_dict = NULL;
      FS_FILESPEC_HANDLE filespec_catalog = NULL;
      wchar_t output_file[MAX_FILE_PATH] = { 0 };

      FSDK_PDFDoc_GetCatalog(doc, &catalog_dict);
      FSDK_AssociatedFiles_AssociateFile(associated_files, catalog_dict, filespec_txt);
      printf("Associate a text file with catalog dictionary.\n");

      FSDK_AssociatedFiles_GetAssociatedFile(associated_files, catalog_dict, 0, &filespec_catalog);
      swprintf_s(output_file, MAX_FILE_PATH, L"%lscatalog.txt", output_directory);
      FSDK_FileSpec_ExportToFile(filespec_catalog, output_file, &return_result);
    }

    error_code = FSDK_PDFDoc_GetPage(doc, 0, &page);
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &progressive);

    //Associate 1.txt with page dictionary.
    {
      FS_PDFDICTIONARY_HANDLE page_dict = NULL;
      FS_FILESPEC_HANDLE filespec_page = NULL;
      wchar_t output_file[MAX_FILE_PATH] = { 0 };

      FSDK_PDFPage_GetDict(page, &page_dict);
      FSDK_AssociatedFiles_AssociateFile(associated_files, page_dict, filespec_txt);
      printf("Associate a text file with page.\n");

      FSDK_AssociatedFiles_GetAssociatedFile(associated_files, page_dict, 0, &filespec_page);     
      swprintf_s(output_file, MAX_FILE_PATH, L"%page.txt", output_directory);
      FSDK_FileSpec_ExportToFile(filespec_page, output_file, &return_result);
    }

    //Associate 1.txt with annotation dictionary.
    {
      FSRectF rect;
      FS_ANNOT_HANDLE annot = NULL;
      FS_PDFDICTIONARY_HANDLE annot_dict = NULL;
      FS_FILESPEC_HANDLE filespec_annot = NULL;
      wchar_t output_file[MAX_FILE_PATH] = { 0 };

      rect.left = 100;
      rect.bottom = 50;
      rect.right = 220;
      rect.top = 100;

      FSDK_PDFPage_AddAnnot(page, e_FSNote, rect, &annot);
      FSDK_Annot_ResetAppearanceStream(annot, &return_result);

      FSDK_Annot_GetDict(annot, &annot_dict);
      FSDK_AssociatedFiles_AssociateFile(associated_files, annot_dict, filespec_txt);
      printf("Associate a text file with annotation.\n");

      FSDK_AssociatedFiles_GetAssociatedFile(associated_files, annot_dict, 0, &filespec_annot);

      swprintf_s(output_file, MAX_FILE_PATH, L"%lsannotation.txt", output_directory);
      FSDK_FileSpec_ExportToFile(filespec_annot, output_file, &return_result);
    }

    //Create filespec with 2.jpg image.
    FSDK_FileSpec_Create(doc, &filespec_jpg);
    FSDK_FileSpec_SetAssociteFileRelationship(filespec_jpg, e_FSRelationshipSource);
    FSDK_FileSpec_SetFileName(filespec_jpg, L"2.jpg");
    FSDK_FileSpec_SetDescription(filespec_jpg, "jpeg");
    swprintf_s(input_file, MAX_FILE_PATH, L"%ls2.jpg", input_path);
    FSDK_FileSpec_Embed(filespec_jpg, input_file, &return_result);
    FSDK_FileSpec_SetCreationDateTime(filespec_jpg, dateTime);
    FSDK_FileSpec_SetModifiedDateTime(filespec_jpg, dateTime);
    FSDK_FileSpec_SetSubtype(filespec_jpg, "application/octet-stream");

    //Associate 2.jpg with marked content.
    {
      FS_PDFDICTIONARY_HANDLE page_dict = NULL;
      FS_PDFDICTIONARY_HANDLE resource_dict = NULL;
      FS_POSITION position = NULL;
      FS_GRAPHICSOBJECT_HANDLE text_object = NULL;

      FSDK_PDFPage_GetDict(page, &page_dict);
      FSDK_PDFDictionary_GetElement(page_dict, "Resources", &resource_dict);
      if (!resource_dict) return 1;

      FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeText, &position);

      FSDK_GraphicsObjects_GetGraphicsObject(page, position, &text_object);
      if (text_object) {
        FS_MARKEDCONTENT_HANDLE markcontent = NULL;
        FS_FILESPEC_HANDLE filespec_text_object = NULL;
        wchar_t output_file[MAX_FILE_PATH] = { 0 };
        int value = 1;

        FSDK_GraphicsObject_GetMarkedContent(text_object, &markcontent);
        if (!markcontent) return 1;

        FSDK_MarkedContent_GetItemCount(markcontent, &value);
        if (value == 0) {
          FSDK_MarkedContent_AddItem(markcontent, "Associated", NULL, &value);
        }
        FSDK_AssociatedFiles_AssociateFile0(associated_files, text_object, 0, resource_dict, "textobject", filespec_jpg);
        FSDK_GraphicsObjects_GenerateContent(page, &return_result);

        printf("Associate a jpeg file with markcontent.\n");

        FSDK_AssociatedFiles_GetAssociatedFile0(associated_files, text_object, 0, &filespec_text_object);

        swprintf_s(output_file, MAX_FILE_PATH, L"%lstextobject.jpg", output_directory);
        FSDK_FileSpec_ExportToFile(filespec_text_object, output_file, &return_result);
      }
    }

    //Associate 2.jpg with image object.
    {
      FS_POSITION position = NULL;
      FS_IMAGEOBJECT_HANDLE image_oject = NULL;
      FS_FILESPEC_HANDLE filespec_image_oject = NULL;
      wchar_t output_file[MAX_FILE_PATH] = { 0 };

      FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeImage, &position);

      FSDK_GraphicsObjects_GetGraphicsObject(page, position, &image_oject);
      if (image_oject) {
        FS_PDFSTREAM_HANDLE pdfstream = NULL;
        FS_PDFDICTIONARY_HANDLE image_oject_dict = NULL;
        FSDK_ImageObject_GetStream(image_oject, &pdfstream);
        FSDK_PDFStream_GetDictionary(pdfstream, &image_oject_dict);
        FSDK_AssociatedFiles_AssociateFile(associated_files, image_oject_dict, filespec_jpg);
        printf("Associate a jpeg file with image xobject.\n");

        FSDK_AssociatedFiles_GetAssociatedFile(associated_files, image_oject_dict, 0, &filespec_image_oject);
        swprintf_s(output_file, MAX_FILE_PATH, L"%lsimage_x_object.jpg", output_directory);
        FSDK_FileSpec_ExportToFile(filespec_image_oject, output_file, &return_result);
      }
    }

    //Associate 2.jpg with form XObject.
    {
      FS_POSITION position = NULL;
      FS_FORMXOBJECT_HANDLE form_x_object = NULL;
      FSDK_GraphicsObjects_GetFirstGraphicsObjectPosition(page, e_FSTypeFormXObject, &position);

      FSDK_GraphicsObjects_GetGraphicsObject(page, position, &form_x_object);
      if (form_x_object) {
        FS_PDFSTREAM_HANDLE pdfstream = NULL;
        FS_PDFDICTIONARY_HANDLE form_x_object_dict = NULL;
        FS_FILESPEC_HANDLE filespec_form_x_object = NULL;
        wchar_t output_file[MAX_FILE_PATH] = { 0 };

        FSDK_FormXObject_GetStream(form_x_object, &pdfstream);
        FSDK_PDFStream_GetDictionary(pdfstream, &form_x_object_dict);
        FSDK_AssociatedFiles_AssociateFile(associated_files, form_x_object_dict, filespec_jpg);
        printf("Associate a jpeg file with form xobject.\n");

        FSDK_AssociatedFiles_GetAssociatedFile(associated_files, form_x_object_dict, 0, &filespec_form_x_object);

        swprintf_s(output_file, MAX_FILE_PATH, L"%lsform_x_object.jpg", output_directory);
        FSDK_FileSpec_ExportToFile(filespec_form_x_object, output_file, &return_result);
      }
    }

    // Save PDF file
    swprintf_s(output_file, MAX_FILE_PATH, L"%lsassociated_files.pdf", output_directory);
    error_code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
    if (error_code != e_FSErrSuccess) {
      printf("Save doc error!\n");
      FSDK_PDFDoc_Release(doc);
      return 1;
    }
    FSDK_PDFDoc_Release(doc);
  }

  FSDK_Library_Release();
  return err_ret;
}
