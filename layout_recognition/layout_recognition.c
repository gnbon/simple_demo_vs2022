// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to call layout recognition related classes
// for PDF document.

// Include Foxit SDK header files.
#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_layoutrecognition_c.h"

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

int elementcount = 0;

void GetChildFromElement(FS_LRSTRUCTUREELEMENT_HANDLE element, FS_LRELEMENT_HANDLE* elem_array,
                         FSElementType type) {
  int elem_count = 0, i = 0;
  FSDK_LRStructureElement_GetChildCount(element, &elem_count);
  for (i = 0; i < elem_count; i++) {
    FS_LRELEMENT_HANDLE item;
    FSElementType item_type;
    FS_BOOL is_structure_element = 0;

    FSDK_LRStructureElement_GetChild(element, i, &item);
    FSDK_LRElement_GetElementType(item, &item_type);
    if (item_type == type) {
      if (elem_array)
        elem_array[elementcount] = item;
      elementcount++;
    } 

    FSDK_LRElement_IsStructureElement(item, &is_structure_element);
    if (is_structure_element == TRUE) {
      FS_LRSTRUCTUREELEMENT_HANDLE str_item = NULL;
      FSDK_LRStructureElement_Create0(item, &str_item);
      GetChildFromElement(str_item, elem_array, type);
      FSDK_LRStructureElement_Release(str_item);
    }
  }
}

wchar_t* LR_Format(FS_BOOL val) {
  return val ? L"True" : L"False";
}

void WriteUnicode(FILE* file, const wchar_t* text_content) {
  FS_WSTR str;
  FS_BSTR dst_bstr;

  str.str = (wchar_t*)text_content;
  str.len = wcslen(text_content);
  FSDK_WStr_UTF16LEEncode(&str, &dst_bstr);

  if (dst_bstr.len == 0) return;
  fwrite((const char*)dst_bstr.str, sizeof(char), dst_bstr.len, file);
}

void WriteUnicodeWithCount(FILE* file, int count, const wchar_t* prefix, const wchar_t* text_content) {
  int i = 0;
  for (i = 0; i < count; i++) {
    WriteUnicode(file, prefix);
  }
  WriteUnicode(file, text_content);
}

void WriteByteString(FILE* file, const char* file_content) {
  FS_BSTR str;
  FS_WSTR wstr;
  str.str = (char*)file_content;
  str.len = strlen(file_content);
  FSDK_BStr_UTF8Decode(&str, &wstr);

  WriteUnicode(file, wstr.str);

  FSDK_WStr_Clear(&wstr);
}

void WriteByteStringWithCount(FILE* file, int count, const char* prefix, const char* text_content) {
  int i = 0;
  FS_BSTR str;
  FS_WSTR wstr;
  str.str = (char*)prefix;
  str.len = strlen(prefix);
  FSDK_BStr_UTF8Decode(&str, &wstr);
  for (i = 0; i < count; i++) {
    WriteUnicode(file, wstr.str);
  }
  FSDK_WStr_Clear(&wstr);

  str.str = (char*)text_content;
  str.len = strlen(text_content);
  FSDK_BStr_UTF8Decode(&str, &wstr);

  WriteUnicode(file, wstr.str);

  FSDK_WStr_Clear(&wstr);
}

void OutputLRStructureElement(FS_LRSTRUCTUREELEMENT_HANDLE element, FILE* file, int depth) {
  FS_LRSTRUCTUREELEMENT_HANDLE parent_element;
  FS_BOOL is_empty;
  FSElementType elem_type;
  FS_BSTR element_type_str;
  FS_INT32 size, i = 0;
  char output_str[MAX_FILE_PATH];

  if (depth > 32)
    return;
  if (element == NULL)
    return;

  FSDK_LRStructureElement_GetParentElement(element, &parent_element);
  is_empty = parent_element == NULL ? TRUE : FALSE;

  FSDK_LRElement_GetElementType(element, &elem_type);
  FSDK_BStr_Init(&element_type_str);
  FSDK_LRElement_StringifyElementType(elem_type, &element_type_str);

  sprintf_s(output_str, MAX_FILE_PATH, "< StructureElement: %s >\r\n", element_type_str.str);
  WriteByteStringWithCount(file, depth, "\t", output_str);

  FSDK_LRStructureElement_GetSupportedAttributeCount(element, &size);

  for (i = 0; i < size; i++) {
    FSAttributeType attr_type;
    FSAttributeValueType attr_value_type;
    int length = 0;
    int idx = 0;
    FS_BOOL is_array;
    FS_BSTR key;
    wchar_t sz_val[MAX_FILE_BUFFER] = {0};
    FSAttributeValueEnum attr_value_enum;
    FS_WSTR wstrkey;
    wchar_t woutput_str[MAX_FILE_BUFFER];

    FSDK_LRStructureElement_GetSupportedAttribute(element, i, &attr_type);
    FSDK_LRStructureElement_GetAttributeValueType(element, attr_type, &attr_value_type);
    FSDK_LRStructureElement_IsArrayAttributeValueType(attr_value_type, &is_array);
    FSDK_LRStructureElement_GetAttributeValueCount(element, attr_type, &length);
    if (attr_value_type == e_FSAttributeValueTypeEmpty)
      continue;

    FSDK_BStr_Init(&key);
    FSDK_LRStructureElement_StringifyAttributeType(attr_type, &key);
    
    if (is_array)
      swprintf_s(sz_val, MAX_FILE_BUFFER, L"%ls[", sz_val);

    for (idx = 0; idx < length; idx++) {
      if (idx)
        swprintf_s(sz_val, MAX_FILE_BUFFER, L"%ls, ", sz_val);

      switch (attr_value_type) {
        case e_FSAttributeValueTypeEnum:
        case e_FSAttributeValueTypeEnumArray: {
          FS_BSTR variant;
          FS_WSTR wstr;
          FSDK_LRStructureElement_GetAttributeValueEnum(element, attr_type, idx, &attr_value_enum);
          FSDK_BStr_Init(&variant);
          FSDK_LRStructureElement_StringifyEnumVariant(attr_value_enum, &variant);

          FSDK_WStr_Init(&wstr);
          FSDK_BStr_UTF8Decode(&variant, &wstr);
          swprintf_s(sz_val, MAX_FILE_BUFFER, L"%ls%ls", sz_val, wstr.str);
          FSDK_BStr_Clear(&variant);
          FSDK_WStr_Clear(&wstr);
        }
        break;
        case e_FSAttributeValueTypeInt32:
        case e_FSAttributeValueTypeInt32Array: {
          FS_INT32 attr_value_int32;
          wchar_t buffer[256] = { 0 };
          FSDK_LRStructureElement_GetAttributeValueInt32(element, attr_type, idx, &attr_value_int32);
          swprintf_s(buffer, 256, L"%d", attr_value_int32);
          swprintf_s(sz_val, MAX_FILE_BUFFER, L"%ls%ls", sz_val, buffer);
        }
        break;
        case e_FSAttributeValueTypeFloat:
        case e_FSAttributeValueTypeFloatArray: {
          float attr_value_float;
          wchar_t buffer[256] = { 0 };
          FSDK_LRStructureElement_GetAttributeValueFloat(element, attr_type, idx, &attr_value_float);
          swprintf_s(buffer, 256, L"%.1f", attr_value_float);
          swprintf_s(sz_val, MAX_FILE_BUFFER, L"%ls%ls", sz_val, buffer);
        }
        break;
        case e_FSAttributeValueTypeARGB:
        case e_FSAttributeValueTypeARGBArray: {
          FSDK_ARGB attr_value_argb;
          wchar_t buffer[256] = { 0 };
          FSDK_LRStructureElement_GetAttributeValueARGB(element, attr_type, idx, &attr_value_argb);
          swprintf_s(buffer, 256, L"#%02X%02X%02X", (FS_UINT8)(attr_value_argb >> 16), (FS_UINT8)(attr_value_argb >> 8), (FS_UINT8)(attr_value_argb));
          swprintf_s(sz_val, MAX_FILE_BUFFER, L"%ls%ls", sz_val, buffer);
        }
        break;
        case e_FSAttributeValueTypeWStr:
        case e_FSAttributeValueTypeWStrArray: {
          FS_WSTR str;
          FSDK_LRStructureElement_GetAttributeValueString(element, attr_type, idx, &str);
          swprintf_s(sz_val, MAX_FILE_BUFFER, L"%ls%ls", sz_val, str.str);
          FSDK_WStr_Clear(&str);
        }
        break;
        default:
          break;
      }
    }
    if (is_array)
     swprintf_s(sz_val, MAX_FILE_BUFFER, L"%ls]", sz_val);

    FSDK_WStr_Init(&wstrkey);
    FSDK_BStr_UTF8Decode(&key, &wstrkey);
    swprintf_s(woutput_str, MAX_FILE_BUFFER, L"%ls: %ls\r\n", wstrkey.str, sz_val);
    WriteUnicodeWithCount(file, depth, L"\t", woutput_str);
    FSDK_WStr_Clear(&wstrkey);
  }
}

void OutputLRGraphicsObjectElement(FS_LRGRAPHICSOBJECTELEMENT_HANDLE element, FILE* file, int depth) {
  const wchar_t* output_str = L"< LRGraphicsObjectElement: >\r\n";
  FS_LRGRAPHICSOBJECTELEMENT_HANDLE parent_pageobj = NULL;
  FS_GRAPHICSOBJECT_HANDLE page_obj = NULL;
  FSGraphicsObjectType type = e_FSTypeAll;
  FS_BOOL is_empty = FALSE;
  FSRectF rc_box;
  FS_PDFDICTIONARY_HANDLE dict = NULL;
  int obj_index;
  FSMatrix matrix;
  char buffer[256] = { 0 };
  FSGraphicsObjectType graphics_type;
  FS_WSTR textstr;

  if (element == NULL)
    return;

  FSDK_LRGraphicsObjectElement_GetParentGraphicsObjectElement(element, &parent_pageobj);
  is_empty = (parent_pageobj == NULL) ? TRUE : FALSE;

  WriteUnicodeWithCount(file, depth, L"\t", output_str);

  FSDK_LRGraphicsObjectElement_GetGraphicsObject(element, &page_obj);

  if (page_obj != NULL) FSDK_GraphicsObject_GetType(page_obj, &type);

  FSDK_LRGraphicsObjectElement_GetBBox(element, &rc_box);
  FSDK_LRGraphicsObjectElement_GetDict(element, &dict);

  FSDK_LRGraphicsObjectElement_GetGraphicsObjectIndex(element, &obj_index);

  FSDK_LRGraphicsObjectElement_GetMatrix(element, &matrix);

  sprintf_s(buffer, 256, "BBox: [%.1f,%.1f,%.1f,%.1f]\r\n", rc_box.left, rc_box.top, rc_box.right, rc_box.bottom);

  WriteByteStringWithCount(file, depth, "\t", buffer);
  sprintf_s(buffer, 256, "Matrix: [%.1f,%.1f,%.1f,%.1f,%.1f,%.1f]\r\n", matrix.a, matrix.b, matrix.c, matrix.d, matrix.e,
               matrix.f);
  WriteByteStringWithCount(file, depth, "\t", buffer);
  sprintf_s(buffer, 256, "PageObjectIndex: %d\r\n", obj_index);
  WriteByteStringWithCount(file, depth, "\t", buffer);

  FSDK_GraphicsObject_GetType(page_obj, &graphics_type);
  if (graphics_type != e_FSTypeText) return;

  FSDK_WStr_Init(&textstr);
  FSDK_TextObject_GetText(page_obj, &textstr);

  if (textstr.len != 0) {
    wchar_t filebuffer[MAX_FILE_BUFFER];
    swprintf_s(filebuffer, MAX_FILE_BUFFER, L"Text: %ls\r\n", textstr.str);
    WriteUnicodeWithCount(file, depth, L"\t", filebuffer);
  }
  FSDK_WStr_Clear(&textstr);
}

void OutputLRContentElement(FS_LRCONTENTELEMENT_HANDLE element, FILE* file, int depth) {
  FS_LRGRAPHICSOBJECTELEMENT_HANDLE page_object = NULL;
  FS_LRSTRUCTUREELEMENT_HANDLE parent_page_object = NULL;
  FS_BSTR element_type_str;
  FSElementType elem_type;
  FS_BOOL is_empty;
  FSRectF rc_box;
  int start_pos = 0, length = 0;
  FS_RANGE_HANDLE range_handle = NULL;
  FSMatrix matrix;
  char filebuffer[MAX_FILE_BUFFER];
  wchar_t temp[256] = { 0 };

  if (element == NULL)
    return;

  FSDK_LRContentElement_GetGraphicsObjectElement(element, &page_object);
  is_empty = page_object == NULL ? TRUE : FALSE;

  FSDK_LRContentElement_GetParentElement(element, &parent_page_object);
  FSDK_LRElement_GetElementType(element, &elem_type);
  FSDK_BStr_Init(&element_type_str);
  FSDK_LRElement_StringifyElementType(elem_type, &element_type_str);
  sprintf_s(filebuffer, MAX_FILE_BUFFER, "< LRContentElement: %s >\r\n", element_type_str.str);

  WriteByteStringWithCount(file, depth, "\t", filebuffer);

  FSDK_LRContentElement_GetBBox(element, &rc_box);
  FSDK_LRContentElement_GetGraphicsObjectRange(element, &range_handle);
  if (range_handle != NULL) {
    int segment_end = 0;
    FSDK_Range_GetSegmentStart(range_handle, 0, &start_pos);

    FSDK_Range_GetSegmentEnd(range_handle, 0, &segment_end);
    length = segment_end - start_pos + 1;
  }

  FSDK_LRContentElement_GetMatrix(element, &matrix);
  swprintf_s(temp, 256, L"BBox: [%.1f,%.1f,%.1f,%.1f]\r\n", rc_box.left, rc_box.top, rc_box.right, rc_box.bottom);
  WriteUnicodeWithCount(file, depth, L"\t", temp);
  swprintf_s(temp, 256, L"Matrix: [%.1f,%.1f,%.1f,%.1f,%.1f,%.1f]\r\n", matrix.a, matrix.b, matrix.c, matrix.d, matrix.e,
               matrix.f);
  WriteUnicodeWithCount(file, depth, L"\t", temp);
  swprintf_s(temp, 256, L"StartPos: %d\r\n", start_pos);
  WriteUnicodeWithCount(file, depth, L"\t", temp);
  swprintf_s(temp, 256, L"Length: %d\r\n", length);
  WriteUnicodeWithCount(file, depth, L"\t", temp);

  if (!is_empty)
    OutputLRGraphicsObjectElement(page_object, file, depth + 1);
}

void ShowLRElementInfo(FS_LRSTRUCTUREELEMENT_HANDLE element, FILE* file, int depth) {
  int elem_list_size, i = 0;
  OutputLRStructureElement(element, file, depth);
  FSDK_LRStructureElement_GetChildCount(element, &elem_list_size);
  for (i = 0; i < elem_list_size; i++) {
    FS_LRELEMENT_HANDLE item;
    FSElementType item_type;
    FS_BOOL is_structure_element = 0;
    FS_BOOL is_content_element = 0;
    FS_BOOL is_graphicsobject_element = 0;

    FSDK_LRStructureElement_GetChild(element, i, &item);
    FSDK_LRElement_GetElementType(item, &item_type);
    FSDK_LRElement_IsStructureElement(item, &is_structure_element);
    FSDK_LRElement_IsContentElement(item, &is_content_element);
    FSDK_LRElement_IsGraphicsObjectElement(item, &is_graphicsobject_element);
    if (is_structure_element == 1) {
      FS_LRSTRUCTUREELEMENT_HANDLE src_item = NULL;
      FSDK_LRStructureElement_Create0(item, &src_item);
      ShowLRElementInfo(src_item, file, depth + 1);
      FSDK_LRStructureElement_Release(src_item);
    } else if (is_content_element == 1) {
      FS_LRCONTENTELEMENT_HANDLE src_item = NULL;
      FSDK_LRContentElement_Create0(item, &src_item);
      OutputLRContentElement(src_item, file, depth);
      FSDK_LRContentElement_Release(src_item);
    } else if (is_graphicsobject_element == 1) {
      FS_LRGRAPHICSOBJECTELEMENT_HANDLE src_item = NULL;
      FSDK_LRGraphicsObjectElement_Create0(item, &src_item);
      OutputLRGraphicsObjectElement(item, file, depth);
      FSDK_LRGraphicsObjectElement_Release(src_item);
    }
  }
}

void OutputAllLRElement(FS_LRSTRUCTUREELEMENT_HANDLE element, const wchar_t* info_path) {
  FILE* file;
  unsigned char temp[] = { 0xFF,0xFE };

  _wfopen_s(&file, info_path, L"w+b");
  fwrite(temp, sizeof(unsigned char), 2, file);
  fseek(file, 0, SEEK_END);

  if (element != NULL) {
    ShowLRElementInfo(element, file, 0);
  } else {
    WriteUnicode(file, L"No layout recognition information!\r\n");
  }
  fclose(file);
}

int main(int argc, char* argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH];
  wchar_t input_file[MAX_FILE_PATH];
  wchar_t info_file[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE doc = NULL;
  FS_PDFPAGE_HANDLE page = NULL;
  FS_LRCONTEXT_HANDLE context = NULL;
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  FS_LRSTRUCTUREELEMENT_HANDLE root = NULL;
  FSElementType type;
  int elem_list_size = 0;
  FS_LRELEMENT_HANDLE* elementarray = NULL;

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lslayout_recognition/", output_path);
  _wmkdir(output_directory);
  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
  swprintf_s(info_file, MAX_FILE_PATH, L"%lslayout_recognition_info.txt", output_directory);

  FSDK_PDFDoc_Create0(input_file, &doc);
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  FSDK_PDFDoc_GetPage(doc, 0, &page);
  FSDK_LRContext_Create(page, &context);
  FSDK_LRContext_StartParse(context, NULL, &progressive);
  FSDK_LRContext_GetRootElement(context, &root);
  FSDK_LRElement_GetElementType(root, &type);
  FSDK_LRStructureElement_GetChildCount(root, &elem_list_size);
  printf("RootElement GetChildCount: %d\n", elem_list_size);

  OutputAllLRElement(root, info_file);

  GetChildFromElement(root, elementarray, e_FSElementTypeSpan);
  elementarray = (FS_LRELEMENT_HANDLE*)malloc(sizeof(FS_LRELEMENT_HANDLE) * elementcount);
  elementcount = 0;
  GetChildFromElement(root, elementarray, e_FSElementTypeSpan);
  printf("LRElement::e_ElementTypeSpan count: %d\n", elementcount);
  free(elementarray);

  FSDK_LRContext_Release(context);
  FSDK_PDFPage_Release(page);
  FSDK_PDFDoc_Release(doc);
  FSDK_Library_Release();
  printf("Layout recognition test.\n");

  return err_ret;
}
