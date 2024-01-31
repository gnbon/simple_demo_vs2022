// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to create or open a portfolio PDF file.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include<stdarg.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_filespec_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fx_stream_c.h"
#include "../../../include/fs_portfolio_c.h"

static const char* sn = "NfAuhaR9g2Ta9dFzHjK7gfP0ZVh78p5ds0GYxMy8DY0BdtD0zcIi8g==";
static const char* key = "8f3gFUOMvWsN+XetmKjesLrtEIThig6PK1SFzio0De6nF+n8WIfE4GpXJ/ObuUZQhMZy6ItYbCol4e34EmY4j3UjrWsHiM2bYvsMF/RtgOayVW1q61EN+Z4eSasOxn8lD4dgNQfb4aLhyIBB0YgW1fzxlUOnDvUxWsD/r71KhJUXWNBigEFZeAN2d3RzuOIx1XQ8FoDdc5ImzuLDQGoVLn6xMvAN48ikWuncDi005n/RWIzevMN4mMAySTIf5J78x9Flz/TtHlirHMh1k9zVrlfJaMqU5YJbH/wXh1gnFg6FyiGJIgaFvmokO78f499TEg9nmk1T8Vjhx0NDCeWY9WfHnj6MIr7YyeC6cvGyO+oFRTprBh26vj49EAkKfau34IhEZPI/DiLZ5OgaW1kLCUCcgpyRcJHfoDCe41byVybTM7y8OCBRGiJjAjw8NDsgBq5eJc6C0jQ7p1uiJ8bqv2H0LrhRuG1KAoA7oqX3kEO/Q6j7S3U/+0eAhtvcYoEA6+GJ7LSSnUSRZIdN2njQUHH7yH48L+ao+KG5I+/jB/f19f4Z0xHxrj2Rp2Z9XNFMPsTAfZvbM3io1rJT0bOlnTj6orjPiJLl2xJ+BG4WEITrZoBVSlcw+qGqUX/EG+9DK3bUoiJuGcnojK1b45LTzU3hYzmgAdR7K9VfxbPxFjzRtwRJT47gdNwgQBUoJ/oivk1SucQimRdd/6M+8M71cg7cvKcWonN29IWoz4BMPDLUO7oAt5jg6yxVG9KQVOUHO1Aagq1ykJ7iZJVqTVg8vkv4M+Cihs+/ZE3u65UeMKpMpcW3KBI1DYf/skXYCl9abhNq9cNJwH6dRibfYgSDEPrEBuizYPN5QBx4iIScoR2swxu1PLYQM5e/hNMQjX2rGpUFF9KBnFMKPRULLlQJIgBIQ5K9EKwgstycvXI2mhag5nNpwQpia7XNdDDEdnrQJ2ZPgJbgbPgqUfjKZXSqm6lqYl4WC4Sw/pzp0XkHhra+wyWcRGJdlPEYihkNy9Zih750k45gKqg29GwVfymU123LlI2lt8FIv8ZLHVsjxqZNpR9JIw3f1GYGq7XxXQdSdExpi13C9nyMWS+p3Uvis+lMe0nkDf1F4E9WcGgCBo7cwPhvSyfnIsVc52WPl8Edf8F7xhsoZ2wK8cra7ivfHqW2tBhFwhSZGw/BgIhbH/hdCVql1h4ecTsCmjyBMhKF7rCwMCSyRAp+392EUELXb7dvu1zJ3aJHAJC6vbnZ07BFNoIfwrMRMwdeNzeFECNMt65F2/OBylLDAsbNyBM9DiLy/PzCp2JMHhk7XUT0kA4KyF0XVIKjGljQOBkzd96a5PFn1KFfI1/qzhB8+ECjhF0FOqX7yE/Ev5VMcQ==";

static const wchar_t* output_path = L"../output_files/";
static const wchar_t* input_path = L"../input_files/";

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

FS_BOOL CreatPortfolioPDF(const wchar_t* saved_portfolio_pdf_path) {
  // Create a new blank portfolio PDF.
  FS_PORTFOLIO_HANDLE new_portfolio = NULL;
  FS_BOOL is_empty = FALSE;
  FS_PORTFOLIONODE_HANDLE root_node = NULL;
  FS_PORTFOLIOFOLDERNODE_HANDLE root_folder = NULL;
  wchar_t input_pdf_filename[MAX_FILE_PATH];
  wchar_t input_pdf_file_path[MAX_FILE_PATH];
  FS_PDFDOC_HANDLE pdf_doc = NULL;
  FS_PORTFOLIOFOLDERNODE_HANDLE new_sub_foldernode = NULL;
  wchar_t input_file_path[MAX_FILE_PATH];
  FS_PORTFOLIOFILENODE_HANDLE new_sub_filenode = NULL;
  FS_PDFDOC_HANDLE portfolio_pdf_doc = NULL;
  FS_BOOL ret = FALSE;
  FSErrorCode error_code = e_FSErrSuccess;

  FSDK_Portfolio_CreatePortfolio(&new_portfolio);

  FSDK_Portfolio_IsEmpty(new_portfolio, &is_empty);
  if (TRUE == is_empty) {
    printf("[FAILED] Fail to create a new portfolio PDF.\r\n");
    FSDK_Portfolio_Release(new_portfolio);
    return FALSE;
  }

  // Get the root node.
  FSDK_Portfolio_GetRootNode(new_portfolio, &root_node);
  FSDK_PortfolioNode_IsEmpty(root_node, &is_empty);
  if (TRUE == is_empty) {
    printf("[FAILED] Fail to get the root node.\r\n");
    FSDK_Portfolio_Release(new_portfolio);
    return FALSE;
  }
  // The root node should be a folder node, so transfer to use FS_PORTFOLIOFOLDERNODE_HANDLE .
  FSDK_PortfolioFolderNode_Create(root_node, &root_folder);

  // Pre-load the PDF file which is to be added to new sub folder in portfolio PDF.
  // ATTENTION: please keep the PDF document object valid until portfolio PDF is saved or ends its life-cycle.
  swprintf_s(input_pdf_filename, MAX_FILE_PATH, L"AboutFoxit.pdf");

  swprintf_s(input_pdf_file_path, MAX_FILE_PATH, L"%ls%ls", input_path, input_pdf_filename);
  FSDK_PDFDoc_Create0(input_pdf_file_path, &pdf_doc);
  error_code = FSDK_PDFDoc_Load(pdf_doc, NULL);
  if (error_code != e_FSErrSuccess) {
    wprintf(L"[FAILED] Fail to load PDF file %ls. Error: %d\n", input_pdf_file_path, error_code);
  }

  // Add a sub folder to root folder node.
  FSDK_PortfolioFolderNode_AddSubFolder(root_folder, L"Sub Folder-1", &new_sub_foldernode);
  FSDK_PortfolioNode_IsEmpty(new_sub_foldernode, &is_empty);
  if (TRUE == is_empty) {
    printf("[FAILED] Fail to add sub folder.\r\n");
  } else {
    FSDK_PortfolioFolderNode_SetDescription(new_sub_foldernode, L"This is a sub folder added to portfolio PDF file.");

    FSDK_PDFDoc_IsEmpty(pdf_doc, &is_empty);
    if (FALSE == is_empty) {
      // Add a valid PDF document object to current folder node.
      // ATTENTION: please keep the PDF document object valid until portfolio PDF is saved or ends its life-cycle.
      FS_PORTFOLIOFILENODE_HANDLE new_filenode = NULL;
      FSDK_PortfolioFolderNode_AddPDFDoc(new_sub_foldernode, pdf_doc, input_pdf_filename, &new_filenode);
      FSDK_PortfolioNode_IsEmpty(new_filenode, &is_empty);
      if (TRUE == is_empty) {
        wprintf(L"[FAILED] Fail to add PDF file %ls.\r\n", input_pdf_file_path);
      } else {
        FS_FILESPEC_HANDLE file_spec = NULL;
        FSDK_PortfolioFileNode_GetFileSpec(new_filenode, &file_spec);
        FSDK_FileSpec_SetDescription(file_spec, "This is a common PDF file added to portfolio PDF file");
        FSDK_FileSpec_Release(file_spec);
      }
      FSDK_PortfolioFolderNode_Release(new_filenode);
    }
  }

  // Add a non-PDF file to root folder node.
  swprintf_s(input_file_path, MAX_FILE_PATH, L"%lsFoxitLogo.jpg", input_path);

  FSDK_PortfolioFolderNode_AddFile(root_folder, input_file_path, &new_sub_filenode);
  FSDK_PortfolioNode_IsEmpty(new_sub_filenode, &is_empty);
  if (TRUE == is_empty) {
    wprintf(L"[FAILED] Fail to add file %ls.\r\n", input_file_path);
  } else {
    FS_FILESPEC_HANDLE file_spec = NULL;
    FSDK_PortfolioFileNode_GetFileSpec(new_sub_filenode, &file_spec);
    FSDK_FileSpec_SetDescription(file_spec, "This is a non-PDF file added to portfolio PDF file.");
    FSDK_FileSpec_Release(file_spec);
  }

  // User can update schema field and other properties by class Portfolio, if necessary.

  // Save the new portfolio PDF file.
  FSDK_Portfolio_GetPortfolioPDFDoc(new_portfolio, &portfolio_pdf_doc);
  FSDK_PDFDoc_IsEmpty(portfolio_pdf_doc, &is_empty);
  if (TRUE == is_empty) {
    printf("[FAILED] Fail to get portfolio PDF document object.\r\n");
  } else {
    FSDK_PDFDoc_SaveAs(portfolio_pdf_doc, saved_portfolio_pdf_path, e_FSSaveFlagsSaveFlagNormal, &ret);
  }
  FSDK_PDFDoc_Release(portfolio_pdf_doc);
  FSDK_PortfolioFileNode_Release(new_sub_filenode);
  FSDK_PortfolioFolderNode_Release(new_sub_foldernode);
  FSDK_PDFDoc_Release(pdf_doc);
  FSDK_PortfolioFolderNode_Release(root_folder);
  FSDK_PortfolioNode_Release(root_node);
  FSDK_Portfolio_Release(new_portfolio);
  return ret;
}

void OutputTab(FILE* file, int tab_count) {
  int i = 0;
  for (i = 0; i<tab_count; i++)
    WriteByteString(file, "\t");
}

void OutputFileNodeInfo(FILE* file, FS_PORTFOLIOFILENODE_HANDLE node, int tab_count) {
  FS_WSTR key_name;
  FS_WSTR file_name;
  FS_FILESPEC_HANDLE file_spec = NULL;
  FS_BSTR description;

  if (tab_count>0)
    OutputTab(file, tab_count);
  WriteByteString(file, "Type:File\r\n");

  FSDK_WStr_Init(&key_name);
  FSDK_PortfolioFileNode_GetKeyName(node, &key_name);
  if (tab_count>0)
    OutputTab(file, tab_count);
  Write(file, L"Key Name:%ls\r\n", key_name.str);
  FSDK_WStr_Clear(&key_name);

  FSDK_PortfolioFileNode_GetFileSpec(node, &file_spec);
  FSDK_WStr_Init(&file_name);
  FSDK_FileSpec_GetFileName(file_spec, &file_name);
  if (tab_count>0)
    OutputTab(file, tab_count);
  Write(file, L"File Name:%ls\r\n", file_name.str);
  FSDK_WStr_Clear(&file_name);

  FSDK_BStr_Init(&description);
  FSDK_FileSpec_GetDescription(file_spec, &description);
  if (tab_count>0)
    OutputTab(file, tab_count);
  WriteByteString(file, "Description:%s\r\n", description.str);
  FSDK_BStr_Clear(&description);

  FSDK_FileSpec_Release(file_spec);
}

void OutputSubNodesInfo(FILE* file, FS_PORTFOLIONODE_HANDLE* portfolionode_array, FS_UINT32 array_length, int tab_count);

void OutputFolderNodeInfo(FILE* file, FS_PORTFOLIOFOLDERNODE_HANDLE node, int tab_count) {
  FS_WSTR name;
  FS_WSTR description;
  FS_PORTFOLIONODE_HANDLE* sub_node_array = NULL;
  FS_UINT32 array_length = 0;
  FS_UINT32 i = 0;

  if (tab_count>0)
    OutputTab(file, tab_count);
  WriteByteString(file, "Type:Folder\r\n");

  FSDK_WStr_Init(&name);
  FSDK_PortfolioFolderNode_GetName(node, &name);
  if (tab_count>0)
    OutputTab(file, tab_count);
  Write(file, L"Name:%ls\r\n", name.str);
  FSDK_WStr_Clear(&name);

  FSDK_WStr_Init(&description);
  FSDK_PortfolioFolderNode_GetDescription(node, &description);
  if (tab_count>0)
    OutputTab(file, tab_count);
  Write(file, L"Description:%ls\r\n", description.str);
  FSDK_WStr_Clear(&description);

  FSDK_PortfolioFolderNode_GetSortedSubNodes(node, sub_node_array, &array_length);
  sub_node_array = (FS_PORTFOLIONODE_HANDLE*)malloc(array_length * sizeof(FS_PORTFOLIONODE_HANDLE));
  FSDK_PortfolioFolderNode_GetSortedSubNodes(node, sub_node_array, &array_length);
  OutputSubNodesInfo(file, sub_node_array, array_length, tab_count + 1);

  for (i = 0; i < array_length; i++) {
    FSDK_PortfolioNode_Release(sub_node_array[i]);
  }
  free(sub_node_array);
}

void OutputNodeInfo(FILE* file, FS_PORTFOLIONODE_HANDLE node, int tab_count) {
  FSPortfolioNodeType node_type = e_FSPortfolioNodeTypeTypeUnknown;
  FSDK_PortfolioNode_GetNodeType(node, &node_type);
  switch (node_type) {
    case e_FSTypeFolder: {
      FS_PORTFOLIOFOLDERNODE_HANDLE folder_node = NULL;
      FSDK_PortfolioFolderNode_Create(node, &folder_node);
      OutputFolderNodeInfo(file, folder_node, tab_count);
      FSDK_PortfolioFolderNode_Release(folder_node);
      break;
    }
    case e_FSTypeFile: {
      FS_PORTFOLIOFILENODE_HANDLE file_node = NULL;
      FSDK_PortfolioFileNode_Create(node, &file_node);
      OutputFileNodeInfo(file, file_node, tab_count);
      FSDK_PortfolioFileNode_Release(file_node);
      break;
    }
    default:
      break;
  }
}

void OutputSubNodesInfo(FILE* file, FS_PORTFOLIONODE_HANDLE* portfolionode_array, FS_UINT32 array_length, int tab_count) {
  FS_UINT32 index = 0;
  if (!portfolionode_array) return;
  for (index = 0; index < array_length; index++) {
    if (tab_count > 0)
      OutputTab(file, tab_count);
    WriteByteString(file, "Sorted Index (under current folder):%d\r\n", index);
    OutputNodeInfo(file, portfolionode_array[index], tab_count);
    WriteByteString(file, "========\r\n");
  }
}

void OutputSchemaFields(FILE* file, FS_SCHEMAFIELD_HANDLE* schemafield_array, FS_UINT32 array_length) {
  FS_UINT32 i = 0;
  if (!schemafield_array || array_length <= 0) return;
  WriteByteString(file, "==== Schema Fields ====\r\n");

  for (i = 0; i < array_length; i++) {
    FS_BOOL is_empty = TRUE;
	FS_BSTR key_name;
    FS_BSTR subtype_name;
    FS_WSTR display_name;
    FS_BOOL is_visible = FALSE;
    FS_SCHEMAFIELD_HANDLE field = schemafield_array[i];

    WriteByteString(file, "Field index:%d\r\n", i);
    FSDK_SchemaField_IsEmpty(field, &is_empty);
    if (TRUE == is_empty) continue;

    FSDK_BStr_Init(&key_name);
    FSDK_SchemaField_GetKeyName(field, &key_name);
    WriteByteString(file, "Key name: %s\r\n", key_name.str);
    FSDK_BStr_Clear(&key_name);

    FSDK_BStr_Init(&subtype_name);
    FSDK_SchemaField_GetSubtypeName(field, &subtype_name);
    WriteByteString(file, "Subtype name: %s\r\n", subtype_name.str);
    FSDK_BStr_Clear(&subtype_name);

    FSDK_WStr_Init(&display_name);
    FSDK_SchemaField_GetDisplayName(field, &display_name);
    Write(file, L"Display name: %ls\r\n", display_name.str);
    FSDK_WStr_Clear(&display_name);

    FSDK_SchemaField_IsVisible(field, &is_visible);;
    WriteByteString(file, "Visibility: %s\r\n", is_visible ? "TRUE" : "FALSE");

    WriteByteString(file, "========\r\n");
  }
}

void OutputPortfolioProperties(FILE* file, FS_PORTFOLIO_HANDLE portfolio) {
  FS_WSTR initial_filespec_keyname;
  FS_SCHEMAFIELD_HANDLE* schemafield_array = NULL;
  FS_UINT32 array_length = 0, i = 0;
  FSInitialViewMode view_mode = e_FSInitialViewModeInitialViewUnknownMode;
  FS_BOOL is_ascending = FALSE;
  FS_BSTR sorting_field_name;
  wchar_t* view_mode_str;

  WriteByteString(file, "==== Portfolio Properties ====\r\n");
  FSDK_WStr_Init(&initial_filespec_keyname);
  FSDK_Portfolio_GetInitialFileSpecKeyName(portfolio, &initial_filespec_keyname);;
  Write(file, L"Initial FS_FILESPEC_HANDLE Key Name:%ls\r\n", initial_filespec_keyname.str);

  FSDK_Portfolio_GetInitialViewMode(portfolio, &view_mode);

  switch (view_mode) {
  case e_FSInitialViewModeInitialViewUnknownMode:
    view_mode_str = L"Unknown";
    break;
  case e_FSInitialViewDetailMode:
    view_mode_str = L"Detail";
    break;
  case e_FSInitialViewTileMode:
    view_mode_str = L"Tile";
    break;
  case e_FSInitialViewHidden:
    view_mode_str = L"Hidden";
    break;
  }
  Write(file, L"Initial View Mode:%ls\r\n", view_mode_str);

  FSDK_Portfolio_IsSortedInAscending(portfolio, &is_ascending);
  WriteByteString(file, "Sorting Order:%s\r\n", is_ascending ? "Ascending" : "Descending");

  FSDK_BStr_Init(&sorting_field_name);
  FSDK_Portfolio_GetSortingFieldKeyName(portfolio, &sorting_field_name);
  WriteByteString(file, "Sorting Field Key Name:%s\r\n", sorting_field_name.str);
  FSDK_BStr_Clear(&sorting_field_name);

  FSDK_Portfolio_GetSchemaFields(portfolio, schemafield_array, &array_length);
  schemafield_array = (FS_SCHEMAFIELD_HANDLE*)malloc(sizeof(FS_SCHEMAFIELD_HANDLE) * array_length);
  FSDK_Portfolio_GetSchemaFields(portfolio, schemafield_array, &array_length);
  OutputSchemaFields(file, schemafield_array, array_length);

  for (i = 0; i < array_length; i++) {
    FSDK_SchemaField_Release(schemafield_array[i]);
  }
  free(schemafield_array);
}

void OutputPortfolioPDFInfo(const wchar_t* portfolio_file_path, FILE* file) {
  FS_PDFDOC_HANDLE pdf_doc = NULL;
  FSErrorCode error_code = e_FSErrSuccess;
  FS_BOOL is_portfolio = FALSE;
  FS_PORTFOLIO_HANDLE exist_portfolio = NULL;
  FS_BOOL is_empty = TRUE;
  FS_PORTFOLIONODE_HANDLE root_node = NULL;
  FS_PORTFOLIOFOLDERNODE_HANDLE root_folder = NULL;
  FS_PORTFOLIONODE_HANDLE* sub_node_array = NULL;
  FS_UINT32 array_length = 0, i = 0;
  FSDK_PDFDoc_Create0(portfolio_file_path, &pdf_doc);
  error_code = FSDK_PDFDoc_Load(pdf_doc, NULL);
  if (error_code != e_FSErrSuccess) {
    wprintf(L"[FAILED] Fail to load Portfolio PDF file %ls. Error: %d\n", portfolio_file_path, error_code);
    return;
  }

  FSDK_PDFDoc_IsPortfolio(pdf_doc, &is_portfolio);
  if (FALSE == is_portfolio) {
    wprintf(L"[FAILED] Fail to output portfolio information for PDF file %ls, because it is not a portfolio PDF file.\r\n", portfolio_file_path);
    return;
  }

  FSDK_Portfolio_CreatePortfolio0(pdf_doc, &exist_portfolio);
  FSDK_Portfolio_IsEmpty(exist_portfolio, &is_empty);
  if (TRUE == is_empty) {
    printf("[FAILED] Fail to create a portfolio object based an existed portfolio PDF document.\r\n");
    return;
  }

  // Output portfolio properties.
  OutputPortfolioProperties(file, exist_portfolio);
  WriteByteString(file, "======================================================================\r\n");

  // Output all nodes.
  WriteByteString(file, "==== Nodes Information ====\r\n");
  FSDK_Portfolio_GetRootNode(exist_portfolio, &root_node);
  FSDK_PortfolioFolderNode_Create(root_node, &root_folder);
  FSDK_PortfolioFolderNode_GetSortedSubNodes(root_folder, sub_node_array, &array_length);
  sub_node_array = (FS_PORTFOLIONODE_HANDLE*)malloc(sizeof(FS_PORTFOLIONODE_HANDLE) * array_length);
  FSDK_PortfolioFolderNode_GetSortedSubNodes(root_folder, sub_node_array, &array_length);
  OutputSubNodesInfo(file, sub_node_array, array_length, 0);

  for (i = 0; i < array_length; i++) {
    FSDK_PortfolioNode_Release(sub_node_array[i]);
  }
  free(sub_node_array);

  FSDK_PortfolioFolderNode_Release(root_folder);
  FSDK_PortfolioNode_Release(root_node);
  FSDK_Portfolio_Release(exist_portfolio);
  FSDK_PDFDoc_Release(pdf_doc);
}

int main(int argc, char *argv[]) {
  int err_ret = 0;
  wchar_t output_directory[MAX_FILE_PATH];
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t saved_portfolio_pdf_path[MAX_FILE_PATH];

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsportfolio/", output_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(saved_portfolio_pdf_path, MAX_FILE_PATH, L"%lsnew_portfolio.pdf", output_directory);

  // Create a new portfolio PDF file
  if (CreatPortfolioPDF(saved_portfolio_pdf_path)) {
    wchar_t output_info_file_path[MAX_FILE_PATH];
    FILE* file = NULL;
    swprintf_s(output_info_file_path, MAX_FILE_PATH, L"%lsnew_portfolio_info.txt", output_directory);

    _wfopen_s(&file, output_info_file_path, L"w+");
    // Show information of the new portfolio PDF file.
    OutputPortfolioPDFInfo(saved_portfolio_pdf_path, file);
    fclose(file);
  }

  FSDK_Library_Release();
  printf("END: Portfolio demo.\r\n");

  return err_ret;
}

