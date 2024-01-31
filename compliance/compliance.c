// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to verify if a PDF file is PDFA-1a version
// or convert a PDF file to PDFA-1a version.

#include <time.h>
#include <direct.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_compliancecommon_c.h"
#include "../../../include/fs_pdfa_c.h"
#include "../../../include/fs_pdfcompliance_c.h"

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

void WriteWStr(FILE* file, FS_WSTR* text_content) {
  FS_BSTR dst_bstr;
  FSDK_WStr_UTF16LEEncode(text_content, &dst_bstr);

  if (dst_bstr.len == 0) return;
  fwrite((const char*)dst_bstr.str, sizeof(char), dst_bstr.len, file);
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

void OutputPDFAFixupData(FS_RESULTINFORMATION_HANDLE result_info, const wchar_t* output_txt_path) {
  FILE* file = NULL;
  unsigned char temp[] = { 0xFF,0xFE };
  int fixup_count = 0;
  wchar_t temp_string[128];
  int i = 0;

  _wfopen_s(&file, output_txt_path, L"w+b");

  fwrite(temp, sizeof(unsigned char), 2, file);
  fseek(file, 0, SEEK_END);


  FSDK_ResultInformation_GetFixupDataCount(result_info, &fixup_count);

  swprintf_s(temp_string, 128, L"== Fixup Data, count:%d ==\r\n", fixup_count);

  WriteUnicode(file, temp_string);
  for (i = 0; i<fixup_count; i++) {
    FSFixupData fixup_data;
    int reason_count = 0, z = 0;
    fixup_data.reasons_array = NULL;
    FSDK_ResultInformation_GetFixupData(result_info, i, &fixup_data);
    fixup_data.reasons_array = (FS_WSTR*)malloc(sizeof(FS_WSTR) * fixup_data.reasons_array_length);
    FSDK_ResultInformation_GetFixupData(result_info, i, &fixup_data);
    swprintf_s(temp_string, 128, L"Used count:%d\r\n", fixup_data.used_count);

    WriteUnicode(file, temp_string);
    WriteUnicode(file, L"Name:");
    if (fixup_data.name.len > 0)
      WriteWStr(file, &fixup_data.name);
    WriteUnicode(file, L"\r\n");

    WriteUnicode(file, L"Comment:");
    if (fixup_data.comment.len > 0)
      WriteWStr(file, &fixup_data.comment);
    WriteUnicode(file, L"\r\n");

    WriteUnicode(file, L"Reason:");

    reason_count = fixup_data.reasons_array_length;
    if (reason_count < 1) {
      WriteUnicode(file, L"\r\n");
    } else {
      for (z = 0; z<reason_count; z++) {
        WriteUnicode(file, L"\t");
        if (fixup_data.reasons_array[z].len > 0)
          WriteWStr(file, &fixup_data.reasons_array[z]);
        WriteUnicode(file, L"\r\n");
      }
    }

    swprintf_s(temp_string, 128, L"State value:%d\r\n", fixup_data.state);
    WriteUnicode(file, temp_string);
    WriteUnicode(file, L"\r\n");
    for (z = 0; z < reason_count; z++)
      FSDK_WStr_Clear(&fixup_data.reasons_array[z]);
    free(fixup_data.reasons_array);
  }
  fclose(file);
}

void OutputPDFAHitData(FS_RESULTINFORMATION_HANDLE result_info, const wchar_t* output_txt_path) {
  FILE* file = NULL;
  unsigned char temp[] = {0xFF,0xFE};
  int hit_data_count;
  wchar_t temp_string[128];
  int i = 0;
  _wfopen_s(&file, output_txt_path, L"w+b");

  fwrite(temp, sizeof(unsigned char), 2, file);
  fseek(file, 0, SEEK_END);

  FSDK_ResultInformation_GetHitDataCount(result_info, &hit_data_count);

  swprintf_s(temp_string, 128, L"== Hit Data, count:%d ==\r\n", hit_data_count);
  WriteUnicode(file, temp_string);

  for (i = 0; i<hit_data_count; i++) {
    FSHitData hit_data;
    int z = 0;
    int trigger_value_count = 0;
    hit_data.trigger_values_array = NULL;
    FSDK_ResultInformation_GetHitData(result_info, i, &hit_data);
    hit_data.trigger_values_array = (FS_WSTR*)malloc(sizeof(FS_WSTR) * hit_data.trigger_values_array_length);
    FSDK_ResultInformation_GetHitData(result_info, i, &hit_data);

    memset(temp_string, 0, 128);
    swprintf_s(temp_string, 128, L"Triggered count:%d\r\n", hit_data.triggered_count);

    WriteUnicode(file, temp_string);

    WriteUnicode(file, L"Name:");
    if (hit_data.name.len > 0)
      WriteWStr(file, &hit_data.name);
    WriteUnicode(file,L"\r\n");

    WriteUnicode(file, L"Comment:");
    if (hit_data.comment.len > 0)
      WriteWStr(file, &hit_data.comment);
    WriteUnicode(file, L"\r\n");

    WriteUnicode(file, L"Trigger value:");
    trigger_value_count = hit_data.trigger_values_array_length;
    if (trigger_value_count < 1) {
      WriteUnicode(file, L"\r\n");
    }
    else {
      for (z = 0; z<trigger_value_count; z++) {
        WriteUnicode(file, L"\t");
        if (hit_data.trigger_values_array[z].len > 0)
          WriteWStr(file, &hit_data.trigger_values_array[z]);
        WriteUnicode(file, L"\r\n");
      }
    }

    swprintf_s(temp_string, 128, L"Check severity:%d\r\nPage index:%d\r\n", hit_data.severity, hit_data.page_index);

    WriteUnicode(file, temp_string);
    WriteUnicode(file, L"\r\n");

    for (z = 0; z < trigger_value_count; z++)
      FSDK_WStr_Clear(&hit_data.trigger_values_array[z]);
    free(hit_data.trigger_values_array);
  }

  fclose(file);
}

FILE* progressfile = NULL;

void gRelease(void* user_data) {
  if (user_data) free(user_data);
}

void gUpdateCurrentStateData(void* user_data, int current_rate, const wchar_t* current_state_string) {
  wchar_t temp_string[32];
  printf("Current rate: %d\n", current_rate);

  swprintf_s(temp_string, 32, L"Current rate:%d, state str:", current_rate);
  WriteUnicode(progressfile, temp_string);
  WriteUnicode(progressfile, current_state_string);
  WriteUnicode(progressfile, L"\r\n");
}

int main(int argc, char *argv[]) {
  wchar_t output_directory[MAX_FILE_PATH];
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t input_file[MAX_FILE_PATH];

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lscompliance/", output_path);
  _wmkdir(output_directory);

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit.pdf", input_path);

  {
    // "compliance_resource_folder_path" is the path of compliance resource folder. Please refer to Developer Guide for more details.
    const wchar_t* compliance_resource_folder_path = L"";
    // If you use an authorization key for Foxit PDF SDK, please set a valid unlock code string to compliance_engine_unlockcode for ComplianceEngine.
    // If you use an trial key for Foxit PDF SDK, just keep compliance_engine_unlockcode as an empty string.
    const char* compliance_engine_unlockcode = "";

    FS_PDFACOMPLIANCE_HANDLE pdfa_compliance = NULL;
    wchar_t save_pdf_path[MAX_FILE_PATH];
    FS_PDFCOMPLIANCE_HANDLE pdf_compliance = NULL;

    if (wcslen(compliance_resource_folder_path) == 0) {
      printf("compliance_resource_folder_path is still empty. Please set it with a valid path to compliance resource folder path.\n");
      return 1;
    }
    // Initialize compliance engine.
    error_code = FSDK_ComplianceEngine_Initialize(compliance_resource_folder_path, compliance_engine_unlockcode);
    if (error_code != e_FSErrSuccess) {
      switch (error_code) {
      case e_FSErrInvalidLicense:
        printf("[Failed] Compliance module is not contained in current Foxit PDF SDK keys.\n");
        break;
      default:
        printf("[Failed] Fail to initialize compliance engine. Error: %d\n", error_code);
        break;
      }
      return 1;
    }
    printf("ComplianceEngine is initialized.\n");

    // Set custom temp folder path for ComplianceEngine. 
    //ComplianceEngine::SetTempFolderPath(L"");
    // Set languages. If not set language to ComplianceEngine, "English" will be used as default.
    //ComplianceEngine::SetLanguage("Chinese-Simplified");

    FSDK_PDFACompliance_Create(&pdfa_compliance);
    {
      // Verify original PDF file.
      FSProgressCallback* verify_progress_callback = NULL;
	  wchar_t output_progress_file[MAX_FILE_PATH];
      FS_RESULTINFORMATION_HANDLE verify_result_info = NULL;
      wchar_t output_txt_file[MAX_FILE_PATH];

	  printf( "======== PDFACompliance: Verify before convert ========\n");
      verify_progress_callback = (FSProgressCallback*)malloc(sizeof(FSProgressCallback));
      verify_progress_callback->user_data = verify_progress_callback;
      verify_progress_callback->Release = gRelease;
      verify_progress_callback->UpdateCurrentStateData = gUpdateCurrentStateData;
      

      swprintf_s(output_progress_file, MAX_FILE_PATH, L"%ls0_pdfacompliance_verify_before_convert_progressdata.txt", output_directory);
      _wfopen_s(&progressfile, output_progress_file, L"w+b");

      FSDK_PDFACompliance_Verify(pdfa_compliance, e_FSVersionPDFA1a, input_file, 0, -1, verify_progress_callback, &verify_result_info);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls0_pdfacompliance_verify_before_convert_hitdata.txt", output_directory);

      OutputPDFAHitData(verify_result_info, output_txt_file);

      FSDK_ResultInformation_Release(verify_result_info);

      fclose(progressfile);
      progressfile = NULL;
    }

    swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%ls1_pdfacompliance_convert_to_pdf1a.pdf", output_directory);
    {
      // Convert original PDF file to PDFA-1a version.
      wchar_t output_progress_file[MAX_FILE_PATH];
      FSProgressCallback* convert_progress_callback = NULL;
      FS_RESULTINFORMATION_HANDLE convert_result_info = NULL;
      wchar_t output_txt_file[MAX_FILE_PATH];

      printf("======== PDFACompliance: Convert ========\n");
      swprintf_s(output_progress_file, MAX_FILE_PATH, L"%ls1_1_pdfacompliance_convert_progressdata.txt", output_directory);
      _wfopen_s(&progressfile, output_progress_file, L"w+b");

      convert_progress_callback = (FSProgressCallback*)malloc(sizeof(FSProgressCallback));
      convert_progress_callback->user_data = convert_progress_callback;
      convert_progress_callback->Release = gRelease;
      convert_progress_callback->UpdateCurrentStateData = gUpdateCurrentStateData;
      
      FSDK_PDFACompliance_ConvertPDFFile(pdfa_compliance, input_file, save_pdf_path, e_FSVersionPDFA1a, convert_progress_callback, &convert_result_info);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls1_1_pdfacompliance_convert_pdfa1a_fixupdata.txt", output_directory);

      OutputPDFAFixupData(convert_result_info, output_txt_file);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls1_1_pdfacompliance_convert_pdfa1a_hitdata.txt", output_directory);

      OutputPDFAHitData(convert_result_info, output_txt_file);

      FSDK_ResultInformation_Release(convert_result_info);

      fclose(progressfile);
      progressfile = NULL;
    }

    {
      // Verify converted result PDF file, which is not expected to be PDFA-1a compliant as the original did not have accessibility structures.
      wchar_t output_progress_file[MAX_FILE_PATH];
      FSProgressCallback* verify_progress_callback_2 = NULL;
      FS_RESULTINFORMATION_HANDLE verify_result_info_2 = NULL;
      wchar_t output_txt_file[MAX_FILE_PATH];

      printf("======== PDFACompliance: Verify after convert ========\n");

      swprintf_s(output_progress_file, MAX_FILE_PATH, L"%ls1_2_pdfacompliance_verify_after_convert_progressdata.txt", output_directory);
      _wfopen_s(&progressfile, output_progress_file, L"w+b");

      verify_progress_callback_2 = (FSProgressCallback*)malloc(sizeof(FSProgressCallback));
      verify_progress_callback_2->user_data = verify_progress_callback_2;
      verify_progress_callback_2->Release = gRelease;
      verify_progress_callback_2->UpdateCurrentStateData = gUpdateCurrentStateData;

      FSDK_PDFACompliance_Verify(pdfa_compliance, e_FSVersionPDFA1a, save_pdf_path, 0, -1, verify_progress_callback_2, &verify_result_info_2);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls1_2_pdfacompliance_verify_after_convert_hitdata.txt", output_directory);

      OutputPDFAHitData(verify_result_info_2, output_txt_file);

      FSDK_ResultInformation_Release(verify_result_info_2);
      fclose(progressfile);
      progressfile = NULL;
    }

    swprintf_s(input_file, MAX_FILE_PATH, L"%lsAboutFoxit_Structured.pdf", input_path);
    {
      // Verify original PDF file.
      FSProgressCallback* verify_progress_callback = NULL;
      wchar_t output_progress_file[MAX_FILE_PATH];
      FS_RESULTINFORMATION_HANDLE verify_result_info;
      wchar_t output_txt_file[MAX_FILE_PATH];

      printf("======== PDFACompliance: Verify before convert ========\n");

      verify_progress_callback = (FSProgressCallback*)malloc(sizeof(FSProgressCallback));
      verify_progress_callback->user_data = verify_progress_callback;
      verify_progress_callback->Release = gRelease;
      verify_progress_callback->UpdateCurrentStateData = gUpdateCurrentStateData;

      swprintf_s(output_progress_file, MAX_FILE_PATH, L"%ls2_pdfacompliance_verify_before_convert_progressdata.txt", output_directory);
      _wfopen_s(&progressfile, output_progress_file, L"w+b");

      FSDK_PDFACompliance_Verify(pdfa_compliance, e_FSVersionPDFA1a, input_file, 0, -1, verify_progress_callback, &verify_result_info);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls2_pdfacompliance_verify_before_convert_hitdata.txt", output_directory);

      OutputPDFAHitData(verify_result_info, output_txt_file);

      FSDK_ResultInformation_Release(verify_result_info);
      fclose(progressfile);
      progressfile = NULL;
    }

    swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%ls3_pdfacompliance_convert_to_pdf1a.pdf", output_directory);
    {
      // Convert original PDF file to PDFA-1a version.
      FSProgressCallback* convert_progress_callback = NULL;
      wchar_t output_progress_file[MAX_FILE_PATH];
      FS_RESULTINFORMATION_HANDLE convert_result_info = NULL;
      wchar_t output_txt_file[MAX_FILE_PATH];
      printf("======== PDFACompliance: Convert ========\n");

      convert_progress_callback = (FSProgressCallback*)malloc(sizeof(FSProgressCallback));
      convert_progress_callback->user_data = convert_progress_callback;
      convert_progress_callback->Release = gRelease;
      convert_progress_callback->UpdateCurrentStateData = gUpdateCurrentStateData;

      swprintf_s(output_progress_file, MAX_FILE_PATH, L"%ls3_1_pdfacompliance_convert_progressdata.txt", output_directory);
      _wfopen_s(&progressfile, output_progress_file, L"w+b");

      FSDK_PDFACompliance_ConvertPDFFile(pdfa_compliance, input_file, save_pdf_path, e_FSVersionPDFA1a, convert_progress_callback, &convert_result_info);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls3_1_pdfacompliance_convert_pdfa1a_fixupdata.txt", output_directory);

      OutputPDFAFixupData(convert_result_info, output_txt_file);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls3_1_pdfacompliance_convert_pdfa1a_hitdata.txt", output_directory);
      OutputPDFAHitData(convert_result_info, output_txt_file);

      FSDK_ResultInformation_Release(convert_result_info);
      fclose(progressfile);
      progressfile = NULL;
    }

    {
      // Verify converted result PDF file, which is expected to be PDFA-1a version.
      FSProgressCallback* verify_progress_callback_2 = NULL;
      wchar_t output_progress_file[MAX_FILE_PATH];
      FS_RESULTINFORMATION_HANDLE verify_result_info_2 = NULL;
      wchar_t output_txt_file[MAX_FILE_PATH];

      printf("======== PDFACompliance: Verify after convert ========\n");

      verify_progress_callback_2 = (FSProgressCallback*)malloc(sizeof(FSProgressCallback));
      verify_progress_callback_2->user_data = verify_progress_callback_2;
      verify_progress_callback_2->Release = gRelease;
      verify_progress_callback_2->UpdateCurrentStateData = gUpdateCurrentStateData;

      swprintf_s(output_progress_file, MAX_FILE_PATH, L"%ls3_2_pdfacompliance_verify_after_convert_progressdata.txt", output_directory);
      _wfopen_s(&progressfile, output_progress_file, L"w+b");

      FSDK_PDFACompliance_Verify(pdfa_compliance, e_FSVersionPDFA1a, save_pdf_path, 0, -1, verify_progress_callback_2, &verify_result_info_2);\
      
      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls3_2_pdfacompliance_verify_after_convert_hitdata.txt", output_directory);

      OutputPDFAHitData(verify_result_info_2, output_txt_file);

      FSDK_ResultInformation_Release(verify_result_info_2);
      fclose(progressfile);
      progressfile = NULL;
    }

    FSDK_PDFCompliance_Create(&pdf_compliance);

    swprintf_s(input_file, MAX_FILE_PATH, L"%lsAF_ImageXObject_FormXObject.pdf", input_path);
    {
      // Convert original PDF file to PDF-1.4.
      FSProgressCallback* convert_progress_callback = NULL;
      wchar_t output_progress_file[MAX_FILE_PATH];
      FS_RESULTINFORMATION_HANDLE convert_result_info = NULL;
      wchar_t output_txt_file[MAX_FILE_PATH];
      printf("======== PDFCompliance: Convert ========\n");

      convert_progress_callback = (FSProgressCallback*)malloc(sizeof(FSProgressCallback));
      convert_progress_callback->user_data = convert_progress_callback;
      convert_progress_callback->Release = gRelease;
      convert_progress_callback->UpdateCurrentStateData = gUpdateCurrentStateData;

      swprintf_s(output_progress_file, MAX_FILE_PATH, L"%ls4_pdfcompliance_convert_1_4_progressdata.txt", output_directory);
      _wfopen_s(&progressfile, output_progress_file, L"w+b");

      swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%ls4_pdfcompliance_convert_to_1_4.pdf", output_directory);

      FSDK_PDFCompliance_ConvertPDFFile(pdf_compliance, input_file, save_pdf_path, 14, convert_progress_callback, &convert_result_info);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls4_pdfcompliance_convert_1_4_fixupdata.txt", output_directory);

      OutputPDFAFixupData(convert_result_info, output_txt_file);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls4_pdfcompliance_convert_1_4_hitdata.txt", output_directory);
      OutputPDFAHitData(convert_result_info, output_txt_file);

      FSDK_ResultInformation_Release(convert_result_info);
      fclose(progressfile);
      progressfile = NULL;
    }
    {
      // Convert original PDF file to PDF-1.7.
      FSProgressCallback* convert_progress_callback = NULL;
      wchar_t output_progress_file[MAX_FILE_PATH];
      FS_RESULTINFORMATION_HANDLE convert_result_info = NULL;
      wchar_t output_txt_file[MAX_FILE_PATH];
      printf("======== PDFCompliance: Convert ========\n");

      convert_progress_callback = (FSProgressCallback*)malloc(sizeof(FSProgressCallback));
      convert_progress_callback->user_data = convert_progress_callback;
      convert_progress_callback->Release = gRelease;
      convert_progress_callback->UpdateCurrentStateData = gUpdateCurrentStateData;

      swprintf_s(output_progress_file, MAX_FILE_PATH, L"%ls5_pdfcompliance_convert_1_7_progressdata.txt", output_directory);
      _wfopen_s(&progressfile, output_progress_file, L"w+b");

      swprintf_s(save_pdf_path, MAX_FILE_PATH, L"%ls5_pdfcompliance_convert_to_1_7.pdf", output_directory);

      FSDK_PDFCompliance_ConvertPDFFile(pdf_compliance, input_file, save_pdf_path, 17, convert_progress_callback, &convert_result_info);

      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls5_pdfcompliance_convert_1_7_fixupdata.txt", output_directory);

      OutputPDFAFixupData(convert_result_info, output_txt_file);
      swprintf_s(output_txt_file, MAX_FILE_PATH, L"%ls5_pdfcompliance_convert_1_7_hitdata.txt", output_directory);

      OutputPDFAHitData(convert_result_info, output_txt_file);

      FSDK_ResultInformation_Release(convert_result_info);
      fclose(progressfile);
      progressfile = NULL;
    }
    FSDK_PDFCompliance_Release(pdf_compliance);
    // Release compliance engine.
    FSDK_PDFACompliance_Release(pdfa_compliance);
    FSDK_ComplianceEngine_Release();
    FSDK_Library_Release();
    printf("== End: Compliance demo. ==\n");
  }

  return 0;
}