// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to operate PDF objects directly.

// Include header files.
// Include  Foxit SDK header files.
#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfobject_c.h"

#include <time.h>
#include<direct.h>
#include <stdio.h>

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

#define FORMAT_UINT32 "%lu"

// This function is to do some operation to PDF object.
void ObjectOperation(FS_PDFDOC_HANDLE document) {
  FS_PDFOBJECT_HANDLE boolean_object = NULL;
  FS_UINT32 boolean_object_number;
  FS_BOOL boolean_object_value;
  FS_PDFOBJECT_HANDLE float_object = NULL;
  FS_UINT32 float_object_number;
  float float_object_value;
  FS_PDFOBJECT_HANDLE integer_object = NULL;
  FS_UINT32 integer_object_number;
  int integer_object_value;
  FS_PDFOBJECT_HANDLE string_object = NULL;
  FS_UINT32 string_object_number;
  FS_WSTR string_object_value;
  FS_PDFOBJECT_HANDLE name_object = NULL;
  FS_UINT32 name_object_number;
  FS_BSTR name_object_value;
  FSDateTime date_time;
  FS_PDFOBJECT_HANDLE datetime_object = NULL;
  FS_UINT32 datetime_object_number;
  FS_WSTR datetime_object_value;
  FS_PDFARRAY_HANDLE array = NULL;
  FS_PDFOBJECT_HANDLE boolean_object_reference = NULL;
  FS_UINT32 boolean_object_reference_number;
  FS_UINT32 return_getobjnum;
  FS_PDFOBJECT_HANDLE string_object_reference = NULL;
  FS_PDFOBJECT_HANDLE name_object_reference = NULL;
  FS_PDFOBJECT_HANDLE datetime_object_reference = NULL;
  FS_UINT32 array_object_number;
  FS_PDFOBJECT_HANDLE float_object_reference = NULL;
  FS_PDFOBJECT_HANDLE integer_object_reference = NULL;
  FS_PDFOBJECT_HANDLE integer_object_direct = NULL;
  FS_PDFOBJECT_HANDLE return_cloneobject = NULL;

  FSDK_PDFObject_CreateFromBoolean(TRUE, &boolean_object);

  FSDK_PDFDoc_AddIndirectObject(document, boolean_object, &boolean_object_number);

  printf("Object number of new boolean object: " FORMAT_UINT32 "\r\n", boolean_object_number);

  FSDK_PDFObject_GetBoolean(boolean_object, &boolean_object_value);
  printf("\tValue of new boolean object: %s\r\n", boolean_object_value ? "true" : "false");

  FSDK_PDFObject_CreateFromFloat(0.1f, &float_object);

  FSDK_PDFDoc_AddIndirectObject(document, float_object, &float_object_number);
  printf("Object number of new number object (as float): " FORMAT_UINT32 "\r\n", float_object_number);

  FSDK_PDFObject_GetFloat(float_object, &float_object_value);
  printf("\tValue of new number object (as float): %f\r\n", float_object_value);

  FSDK_PDFObject_CreateFromInteger(1, &integer_object);

  FSDK_PDFDoc_AddIndirectObject(document, integer_object, &integer_object_number);
  printf("Object number of new number object (as integer):" FORMAT_UINT32 "\r\n", integer_object_number);

  FSDK_PDFObject_GetInteger(integer_object, &integer_object_value);
  printf("\tValue of new number object (as integer): %d\r\n", integer_object_value);

  FSDK_PDFObject_CreateFromString(L"foxit", &string_object);

  FSDK_PDFDoc_AddIndirectObject(document, string_object, &string_object_number);
  printf("Object number of new string object:" FORMAT_UINT32 "\r\n", string_object_number);

  FSDK_WStr_Init(&string_object_value);
  FSDK_PDFObject_GetWideString(string_object, &string_object_value);
  wprintf(L"\tValue of new string object: %ls\r\n", (const wchar_t*)string_object_value.str);
  FSDK_WStr_Clear(&string_object_value);

  FSDK_PDFObject_CreateFromName("sdk", &name_object);

  FSDK_PDFDoc_AddIndirectObject(document, name_object, &name_object_number);
  printf("Object number of new name object:" FORMAT_UINT32 "\r\n", name_object_number);

  FSDK_BStr_Init(&name_object_value);
  FSDK_PDFObject_GetName(name_object, &name_object_value);
  printf("\tValue of new name object: %s\r\n", (const char*)name_object_value.str);
  FSDK_BStr_Clear(&name_object_value);

  date_time.year = 2017;
  date_time.month = 9;
  date_time.day = 27;
  date_time.hour = 19;
  date_time.minute = 36;
  date_time.second = 6;
  date_time.milliseconds = 0;
  date_time.utc_hour_offset = 8;
  date_time.utc_minute_offset = 0;

  FSDK_PDFObject_CreateFromDateTime(date_time, &datetime_object);

  FSDK_PDFDoc_AddIndirectObject(document, datetime_object, &datetime_object_number);
  printf("Object number of new string object (as date-time): " FORMAT_UINT32 "\r\n", datetime_object_number);

  FSDK_WStr_Init(&datetime_object_value);
  FSDK_PDFObject_GetWideString(datetime_object, &datetime_object_value);
  wprintf(L"\tValue of new string object (as date-time): %ls\r\n", (const wchar_t*)datetime_object_value.str);
  FSDK_WStr_Clear(&datetime_object_value);

  FSDK_PDFArray_Create(&array);

  FSDK_PDFObject_CreateReference(document, boolean_object_number, &boolean_object_reference);

  FSDK_PDFObject_GetObjNum(boolean_object_reference, &boolean_object_reference_number);
  printf("Object number of a new reference object to a boolean object:" FORMAT_UINT32 "\r\n", boolean_object_reference_number);

  FSDK_PDFObject_CreateReference(document, float_object_number, &float_object_reference);

  FSDK_PDFObject_CreateReference(document, integer_object_number, &integer_object_reference);

  FSDK_PDFObject_GetDirectObject(integer_object_reference, &integer_object_direct);

  FSDK_PDFObject_GetObjNum(integer_object_direct, &return_getobjnum);
  printf("Object number of the direct number object: " FORMAT_UINT32 "\r\n", return_getobjnum);

  FSDK_PDFObject_CreateReference(document, string_object_number, &string_object_reference);

  FSDK_PDFObject_CreateReference(document, name_object_number, &name_object_reference);

  FSDK_PDFObject_CreateReference(document, datetime_object_number, &datetime_object_reference);
  FSDK_PDFArray_AddElement(array, boolean_object_reference);
  FSDK_PDFArray_AddElement(array, float_object_reference);
  FSDK_PDFArray_AddElement(array, integer_object_reference);
  FSDK_PDFArray_AddElement(array, string_object_reference);
  FSDK_PDFArray_AddElement(array, name_object_reference);
  FSDK_PDFArray_AddElement(array, datetime_object_reference);
  FSDK_PDFObject_CloneObject(boolean_object, &return_cloneobject);
  FSDK_PDFArray_AddElement(array, return_cloneobject);

  FSDK_PDFDoc_AddIndirectObject(document, array, &array_object_number);
  printf("Object number of array object: " FORMAT_UINT32 "\r\n", array_object_number);
}

// This function is to remove some properties from catalog dictionary.
void RemoveCatalogKey(FS_PDFDOC_HANDLE document) {
  FS_BOOL return_isempty;
  FS_PDFDICTIONARY_HANDLE catalog = NULL;
  const char* key_strings[] = { "Type", "Boolean", "Name", "String", "Array", "Dict" };
  int count = 0, i = 0;

  FSDK_PDFDoc_IsEmpty(document, &return_isempty);
  if (return_isempty) return;

  FSDK_PDFDoc_GetCatalog(document, &catalog);
  if (NULL == catalog) return;

  count = sizeof(key_strings) / sizeof(key_strings[0]);
  for (i = 0; i < count; i++) {
    FS_BOOL return_haskey;
    FSDK_PDFDictionary_HasKey(catalog, key_strings[i], &return_haskey);
    if (return_haskey)
      FSDK_PDFArray_RemoveAt(catalog, i);
  }
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH] = { 0 };
  FS_PDFDOC_HANDLE doc = NULL;

  wchar_t output_directory[MAX_FILE_PATH] = { 0 };
  wchar_t save_pdf_path[MAX_FILE_PATH] = { 0 };
  FS_PROGRESSIVE_HANDLE return_startsaveas = NULL;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdfobjects/", output_path);
  _wmkdir(output_directory);

  swprintf_s(input_file, MAX_FILE_PATH, L"%lspdfobjects.pdf", input_path);

  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  // Do some operation about PDF object.
  ObjectOperation(doc);

  swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%lspdfobjects_addnewobjects.pdf", output_directory);

  FSDK_PDFDoc_StartSaveAs(doc, save_pdf_path, e_FSSaveFlagNoOriginal, NULL, &return_startsaveas);

  // To remove some properties from catalog dictionary.
  RemoveCatalogKey(doc);

  swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%lspdfobjects_removekeyfromcatalog.pdf", output_directory);

  FSDK_PDFDoc_StartSaveAs(doc, save_pdf_path, e_FSSaveFlagNoOriginal, NULL, &return_startsaveas);
  FSDK_PDFDoc_Release(doc);

  FSDK_Library_Release();
  printf("pdfobject demo done.\n");
  return err_ret;
}

