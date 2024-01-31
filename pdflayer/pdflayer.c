// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to render all PDF layers of a PDF,
// and add layers in a PDF document.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_render_c.h"
#include "../../../include/fs_pdflayer_c.h"
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

wchar_t output_directory[MAX_FILE_PATH];

void WriteByteString(FILE* file, const char* format, ...) {
  va_list vars;
  if (file) {
    // Get variable list
    va_start(vars, format);
    // Call vfprintf to format log data and output to log file
    vfprintf(file, format, vars);
    // End variable list
    va_end(vars);
  }
}

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

void WriteByteStringWithCount(FILE* file, int count, const char * prefix, const char * format, ...) {
  int i = 0;
  va_list vars;
  for (i = 0; i < count; i++) {
    WriteByteString(file, "%s", prefix);
  }

  if (file) {
    // Get variable list
    va_start(vars, format);
    // Call vfprintf to format log data and output to log file
    vfprintf(file, format, vars);
    // End variable list
    va_end(vars);
  }
}

void WriteWithCount(FILE* file, int count, const wchar_t * prefix, const wchar_t * format, ...) {
  int i = 0;
  va_list vars;

  for (i = 0; i < count; i++) {
    Write(file, L"%s", prefix);
  }

  if (file) {
    // Get variable list
    va_start(vars, format);
    // Call vfprintf to format log data and output to log file
    vfwprintf(file, format, vars);
    // End variable list
    va_end(vars);
  }
}

const void RenderPageToImage(FS_PDFPAGE_HANDLE page, FS_LAYERCONTEXT_HANDLE layer_content, const wchar_t* bitmap_file) {
  FS_BOOL return_isparsed;
  FSErrorCode error_code = e_FSErrSuccess;
  float f_width;
  float f_height;
  int width, height = 0;
  FSRotation return_rotation;
  FSMatrix matrix;
  FS_BITMAP_HANDLE bitmap = NULL;
  FS_RENDERER_HANDLE render = NULL;
  FS_BOOL return_is_empty;
  FS_PROGRESSIVE_HANDLE return_startrender = NULL;
  FS_IMAGE_HANDLE image = NULL;
  FS_BOOL return_addframe;
  FS_BOOL return_saveas;

  error_code = FSDK_PDFPage_IsParsed(page, &return_isparsed);
  if (error_code == e_FSErrSuccess) {
    // Parse page.
    FS_PROGRESSIVE_HANDLE return_value;
    FSDK_PDFPage_StartParse(page, e_FSParseFlagsParsePageNormal, NULL, FALSE, &return_value);
  }

  FSDK_PDFPage_GetWidth(page, &f_width);
  FSDK_PDFPage_GetHeight(page, &f_height);
  width = (int)(f_width);
  height = (int)(f_height);

  FSDK_PDFPage_GetRotation(page, &return_rotation);

  FSDK_PDFPage_GetDisplayMatrix(page, 0, 0, width, height, return_rotation, &matrix);

  // Prepare a bitmap for rendering.
  FSDK_Bitmap_Create(width, height, e_FSDIBArgb, NULL, 0, &bitmap);
  FSDK_Bitmap_FillRect(bitmap, 0xFFFFFFFF, NULL);

  // Render page.
  FSDK_Renderer_Create(bitmap, FALSE, &render);
  FSDK_LayerContext_IsEmpty(render, &return_is_empty);
  if (!return_is_empty) {
    FSDK_Renderer_SetLayerContext(render, layer_content);
  }

  FSDK_Renderer_StartRender(render, page, matrix, NULL, &return_startrender);

  FSDK_Image_Create(&image);
  FSDK_Image_AddFrame(image, bitmap, &return_addframe);
  FSDK_Image_SaveAs(image, bitmap_file, &return_saveas);
  FSDK_Bitmap_Release(bitmap);
  FSDK_Renderer_Release(render);
  FSDK_Image_Release(image);
}

char* UsgaeCodeToString(FSUsageState state) {
  switch (state) {
  case e_FSStateON:
    return "ON";
  case e_FSStateOFF:
    return "OFF";
  case e_FSStateUnchanged:
    return "Unchanged";
  case e_FSStateUndefined:
    return "Undefined";
  }
  return "Unknown";
}

void GetAllLayerNodesInformation(FS_LAYERNODE_HANDLE layer_node, int depth, FILE* file) {
  int count, i = 0;
  if (depth >= 0) {
    FS_WSTR return_getname;
    FS_BOOL return_haslayer;

    FSDK_WStr_Init(&return_getname);
    FSDK_LayerNode_GetName(layer_node, &return_getname);
    WriteWithCount(file, depth, L"\t", L"%ls", return_getname.str);

    FSDK_LayerNode_HasLayer(layer_node, &return_haslayer);
    if (return_haslayer) {
      FSUsageState state;
      FSLayerPrintData print_data;
      FSLayerZoomData zoom_data;
      FSUsageState return_getexportusage;

      FSDK_LayerNode_GetViewUsage(layer_node, &state);
      WriteByteString(file, " %s\r\n", state == e_FSStateON ? "[*]" : "[ ]");
      WriteByteStringWithCount(file, depth, "\t", "View usage state:\t%s\r\n", (const char*)UsgaeCodeToString(state));

      FSDK_LayerNode_GetExportUsage(layer_node, &return_getexportusage);
      WriteByteStringWithCount(file, depth, "\t", "Export usage state:\t%s\r\n", (const char*)UsgaeCodeToString(return_getexportusage));

      FSDK_LayerNode_GetPrintUsage(layer_node, &print_data);
      WriteByteStringWithCount(file, depth, "\t", "Print usage state:\t%s, subtype: %s\r\n", (const char*)UsgaeCodeToString(print_data.print_state),
        (const char*)print_data.subtype.str);

      FSDK_LayerNode_GetZoomUsage(layer_node, &zoom_data);
      WriteByteStringWithCount(file, depth, "\t", "Zoom usage:\tmin_factor = %.4f max_factor = %.4f\r\n\r\n", zoom_data.min_factor,
        zoom_data.max_factor);
    }
    else {
      WriteByteString(file, "\r\n");
    }
  }

  depth++;

  FSDK_LayerNode_GetChildrenCount(layer_node, &count);
  for (i = 0; i < count; i++) {
    FS_LAYERNODE_HANDLE child = NULL;
    FSDK_LayerNode_GetChild(layer_node, i, &child);
    GetAllLayerNodesInformation(child, depth, file);
  }
}

void SetAllLayerNodesInformation(FS_LAYERNODE_HANDLE layer_node) {
  FS_BOOL return_haslayer;
  int count, i = 0;
  FSDK_LayerNode_HasLayer(layer_node, &return_haslayer);
  if (return_haslayer) {
    FS_BOOL return_value;
    FSLayerPrintData print_data;
    FSLayerZoomData zoom_data;
    FS_WSTR return_getname;
    wchar_t new_name[MAX_FILE_PATH];
    FSDK_LayerNode_SetDefaultVisible(layer_node, TRUE, &return_value);
    FSDK_LayerNode_SetExportUsage(layer_node, e_FSStateUndefined, &return_value);;
    FSDK_LayerNode_SetViewUsage(layer_node, e_FSStateOFF, &return_value);

    print_data.print_state = e_FSStateON;
    print_data.subtype.str = "subtype_print";
    print_data.subtype.len = strlen("subtype_print");
    FSDK_LayerNode_SetPrintUsage(layer_node, print_data, &return_value);

    zoom_data.max_factor = 10;
    zoom_data.min_factor = 1;
    FSDK_LayerNode_SetZoomUsage(layer_node, zoom_data, &return_value);

    FSDK_WStr_Init(&return_getname);
    FSDK_LayerNode_GetName(layer_node, &return_getname);

    swprintf(new_name, MAX_FILE_PATH, L"[View_OFF_Print_ON_Export_Undefined]%ls", return_getname.str);
    FSDK_LayerNode_SetName(layer_node, new_name, &return_value);
  }

  FSDK_LayerNode_GetChildrenCount(layer_node, &count);
  for (i = 0; i < count; i++) {
    FS_LAYERNODE_HANDLE child = NULL;
    FSDK_LayerNode_GetChild(layer_node, i, &child);
    SetAllLayerNodesInformation(child);
  }
}

void SetAllLayerNodesVisible(FS_LAYERCONTEXT_HANDLE context, FS_LAYERNODE_HANDLE layer_node, FS_BOOL visible) {
  FS_BOOL return_haslayer;
  int count, i = 0;

  FSDK_LayerNode_HasLayer(layer_node, &return_haslayer);
  if (return_haslayer) {
    FS_BOOL return_value;
    FSDK_LayerContext_SetVisible(context, layer_node, FALSE, &return_value);
  }

  FSDK_LayerNode_GetChildrenCount(layer_node, &count);
  for (i = 0; i < count; i++) {
    FS_LAYERNODE_HANDLE child = NULL;
    FSDK_LayerNode_GetChild(layer_node, i, &child);
    SetAllLayerNodesVisible(context, child, visible);
  }
}

void removeWChar(wchar_t* str, wchar_t c) {
  wchar_t *src = str, *dst = str;
  while (*src) {
    if (*src != c) {
      *dst++ = *src;
    }
    src++;
  }
  *dst = L'\0';
}

void RenderAllLayerNodes(FS_PDFDOC_HANDLE doc, FS_LAYERCONTEXT_HANDLE context, FS_LAYERNODE_HANDLE layer_node, const wchar_t* pdf_name) {
  FS_BOOL return_haslayer;
  int count, i = 0;
  FSDK_LayerNode_HasLayer(layer_node, &return_haslayer);
  if (return_haslayer) {
    FS_BOOL return_value;
    int page_count, i = 0;

    FSDK_LayerContext_SetVisible(context, layer_node, TRUE, &return_value);

    FSDK_PDFDoc_GetPageCount(doc, &page_count);
    for (i = 0; i < page_count; i++) {
      FS_PDFPAGE_HANDLE page = NULL;
      wchar_t s[60];
      FS_WSTR layer_name;
	  wchar_t* strlayername = NULL;
      wchar_t file_name[MAX_FILE_PATH];

      FSErrorCode code = FSDK_PDFDoc_GetPage(doc, i, &page);

      swprintf_s(s, 60, L"%d", i);

      FSDK_WStr_Init(&layer_name);
      FSDK_LayerNode_GetName(layer_node, &layer_name);

      strlayername = layer_name.str;
      removeWChar(strlayername, L':');
      removeWChar(strlayername, L'>');
      removeWChar(strlayername, L'<');
      removeWChar(strlayername, L'=');
 
      swprintf_s(file_name, MAX_FILE_PATH, L"%lspage_%ls_layer_node_%ls.bmp", output_directory, s, strlayername);
      RenderPageToImage(page, context, file_name);
    }
  }

  FSDK_LayerNode_GetChildrenCount(layer_node, &count);
  for (i = 0; i < count; i++) {
    FS_LAYERNODE_HANDLE child = NULL;
    FSDK_LayerNode_GetChild(layer_node, i, &child);
    RenderAllLayerNodes(doc, context, child, pdf_name);
  }
  FSDK_LayerNode_HasLayer(layer_node, &return_haslayer);
  if (return_haslayer) {
    // The visibility of parent layer will affect the children layers,
    // so reset the visibility of parent layer after children have been rendered.
    FS_BOOL return_value;
    FSDK_LayerContext_SetVisible(context, layer_node, FALSE, &return_value);
  }
}

int main(int argc, char *argv[]) {
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE doc = NULL;
  FS_LAYERTREE_HANDLE layertree = NULL;
  FS_LAYERNODE_HANDLE root = NULL;
  FS_BOOL return_value = FALSE;
  wchar_t file_info[MAX_FILE_PATH]; 
  FILE* file = NULL;
  wchar_t output_file[MAX_FILE_PATH];
  FS_BOOL return_result = FALSE;
  FS_LAYERCONTEXT_HANDLE context = NULL;
  int children_count, i = 0;
  wchar_t input_path_new[MAX_FILE_PATH];
  FS_BOOL has_layer;
  int child_count;
  FS_PDFPAGE_HANDLE page = NULL;
  wchar_t input_file_path[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE newdoc = NULL;
  FS_PDFPAGE_HANDLE newpage = NULL;
  FS_PROGRESSIVE_HANDLE progressive = NULL;
  FS_LAYERNODE_HANDLE child = NULL;
  FS_LAYERNODE_HANDLE child0 = NULL;
  FS_LAYERNODE_HANDLE return_addchild;

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lspdflayer/", output_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsOCTest_src.pdf", input_path);

  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  FSDK_LayerTree_Create(doc, &layertree);
  FSDK_LayerTree_GetRootNode(layertree, &root);
  FSDK_LayerTree_IsEmpty(layertree, &return_value);
  if (return_value) {
    printf("No layer information!\r\n");
    return 1;
  }

  // Get original information of all layer nodes.
  swprintf_s(file_info, MAX_FILE_PATH, L"%lsoriginal_layer_informations.txt", output_directory);

  _wfopen_s(&file, file_info, L"w+");
  GetAllLayerNodesInformation(root, -1, file);
  fclose(file);

  // Set new information.
  SetAllLayerNodesInformation(root);
  // Get new information.
  swprintf_s(file_info, MAX_FILE_PATH, L"%lsnew_layer_informations.txt", output_directory);
  _wfopen_s(&file, file_info, L"w+");
  GetAllLayerNodesInformation(root, -1, file);
  fclose(file);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsnew_layers.pdf", output_directory);

  error_code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }
  // Render layer node.
  FSDK_LayerContext_Create(doc, e_FSUsageView, &context);
  SetAllLayerNodesVisible(context, root, FALSE);

  RenderAllLayerNodes(doc, context, root, output_file);

  FSDK_LayerContext_Release(context);
  FSDK_LayerTree_Release(layertree);
  layertree = 0;
  FSDK_PDFDoc_Release(doc);

  // Edit layer tree
  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  FSDK_LayerTree_Create(doc, &layertree);
  FSDK_LayerTree_GetRootNode(layertree, &root);
  FSDK_LayerNode_GetChildrenCount(root, &children_count);
  FSDK_LayerNode_RemoveChild(root, children_count - 1, &return_value);

  FSDK_LayerNode_GetChild(root, children_count - 2, &child);

  FSDK_LayerNode_GetChild(root, 0, &child0);
  FSDK_LayerNode_MoveTo(child, child0, 0, &return_value);

  FSDK_LayerNode_AddChild(root, 0, L"AddedLayerNode", TRUE, &return_addchild);
  FSDK_LayerNode_AddChild(root, 0, L"AddedNode", TRUE, &return_addchild);
  swprintf_s(file_info, MAX_FILE_PATH, L"%lsedit_layer_informations.txt", output_directory);
  _wfopen_s(&file, file_info, L"w+");

  GetAllLayerNodesInformation(root, -1, file);

  swprintf_s(output_file, MAX_FILE_PATH, L"%lsedit_layer_tree.pdf", output_directory);
  error_code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }
  FSDK_LayerTree_Release(layertree);
  layertree = 0;
  FSDK_PDFDoc_Release(doc);
  fclose(file);

  // Create layer tree
  swprintf_s(input_path_new, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);
  error_code = FSDK_PDFDoc_Create0(input_path_new, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  FSDK_PDFDoc_HasLayer(doc, &has_layer);
  FSDK_LayerTree_Create(doc, &layertree);
  FSDK_PDFDoc_HasLayer(doc, &has_layer);
  FSDK_LayerTree_GetRootNode(layertree, &root);

  FSDK_LayerNode_GetChildrenCount(root, &child_count);
  FSDK_LayerNode_AddChild(root, 0, L"AddedLayerNode", TRUE, &return_addchild);
  FSDK_LayerNode_AddChild(root, 0, L"AddedNode", FALSE, &return_addchild);

  swprintf_s(file_info, MAX_FILE_PATH, L"%lscreate_layer_informations.txt", output_directory);
  _wfopen_s(&file, file_info, L"w+");

  GetAllLayerNodesInformation(root, -1, file);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lscreate_layer_tree.pdf", output_directory);
  error_code = FSDK_PDFDoc_SaveAs(doc, output_file, e_FSSaveFlagNoOriginal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    return 1;
  }
  FSDK_LayerTree_Release(layertree);
  layertree = 0;
  FSDK_PDFDoc_Release(doc);

  //Copy graphics objects from A PDF to B PDF
  error_code = FSDK_PDFDoc_Create0(input_file, &doc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(doc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(doc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file, error_code);
    return 1;
  }

  FSDK_PDFDoc_GetPage(doc, 0, &page);

  FSDK_PDFPage_StartParse(page, 0, NULL, FALSE, &progressive);

  swprintf_s(input_file_path, MAX_FILE_PATH, L"%lsSamplePDF.pdf", input_path);

  error_code = FSDK_PDFDoc_Create0(input_file_path, &newdoc);
  if (error_code != e_FSErrSuccess) return 1;
  error_code = FSDK_PDFDoc_Load(newdoc, NULL);
  if (error_code != e_FSErrSuccess) {
    FSDK_PDFDoc_Release(newdoc);
    wprintf(L"The Doc [%ls] Error: %d\n", input_file_path, error_code);
    return 1;
  }

  FSDK_PDFDoc_GetPage(newdoc, 0, &newpage);
  FSDK_PDFPage_StartParse(newpage, 0, NULL, FALSE, &progressive);
  FSDK_PDFDoc_HasLayer(newdoc, &has_layer);
  FSDK_LayerTree_Create(newdoc, &layertree);
  FSDK_PDFDoc_HasLayer(newdoc, &has_layer);
  FSDK_LayerTree_GetRootNode(layertree, &root);
  FSDK_LayerNode_AddChild(root, 0, L"AddedLayerNode", TRUE, &return_addchild);

  for (i = 0; i < 5; i++) {
    FS_POSITION pos = NULL;
    FS_GRAPHICSOBJECT_HANDLE graphics_object = NULL;
    FS_GRAPHICSOBJECT_HANDLE ngraphics_object = NULL;

    FSDK_GraphicsObjects_GetGraphicsObject0(page, i, &graphics_object);

    FSDK_GraphicsObject_Clone(graphics_object, &ngraphics_object);
    FSDK_GraphicsObjects_InsertGraphicsObject(newpage, 0, ngraphics_object, &pos);
    FSDK_LayerNode_AddGraphicsObject(return_addchild, newpage, ngraphics_object, &return_value);
  }
  FSDK_GraphicsObjects_GenerateContent(newpage, &return_value);
  swprintf_s(output_file, MAX_FILE_PATH, L"%lscopy_graphics_objects.pdf", output_directory);

  error_code = FSDK_PDFDoc_SaveAs(newdoc, output_file, e_FSSaveFlagsSaveFlagNormal, &return_result);
  if (error_code != e_FSErrSuccess) {
    printf("Save doc error!\n");
    FSDK_PDFDoc_Release(doc);
    FSDK_PDFDoc_Release(newdoc);
    return 1;
  }
  FSDK_LayerTree_Release(layertree);
  FSDK_PDFDoc_Release(doc);
  FSDK_PDFDoc_Release(newdoc);

  FSDK_Library_Release();
  printf("PDFLayer test.\n");
  return 0;
}
