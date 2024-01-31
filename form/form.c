// Copyright (C) 2003-2023, Foxit Software Inc..
// All Rights Reserved.
//
// http://www.foxitsoftware.com
//
// The following code is copyrighted and contains proprietary information and trade secrets of Foxit Software Inc..
// You cannot distribute any part of Foxit PDF SDK to any third party or general public,
// unless there is a separate license agreement with Foxit Software Inc. which explicitly grants you such rights.
//
// This file contains an example to demonstrate how to use Foxit PDF SDK to add form field and
// get form field information.

// Include Foxit SDK header files.
#include <time.h>
#include<direct.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../include/fs_basictypes_c.h"
#include "../../../include/fs_common_c.h"
#include "../../../include/fs_pdfdoc_c.h"
#include "../../../include/fs_pdfpage_c.h"
#include "../../../include/fs_pdfform_c.h"
#include "../../../include/fs_action_c.h"

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

void AddInteractiveForms(FS_PDFPAGE_HANDLE page, FS_FORM_HANDLE form) {
  {
    // Add push button field.
    FS_CONTROL_HANDLE btn_submit = NULL;
    FSRectF rect;
    FSDefaultAppearance default_ap;
    FS_WIDGETANNOT_HANDLE widget = NULL;
    FS_ACTION_HANDLE action_handle = NULL;
    FS_SUBMITFORMACTION_HANDLE submit_action = NULL;
    FS_PDFDOC_HANDLE doc = NULL;
    int count = 0, i = 0;
    FS_WSTR* name_array = NULL;
    FS_BOOL return_value;

    rect.left = 50;
    rect.bottom = 750;
    rect.right = 150;
    rect.top = 780;
    FSDK_Form_AddControl(form, page, L"Push Button Submit", e_FSTypePushButton, rect, &btn_submit);

    // Set default appearance
    default_ap.flags = e_FSFlagFont | e_FSFlagFontSize | e_FSFlagTextColor;
    FSDK_Font_Create0(e_FSStdIDHelveticaB, &default_ap.font);
    default_ap.text_size = 12.0f;
    default_ap.text_color = 0x000000;
    FSDK_Form_SetDefaultAppearance(form, default_ap);

    // Set push button appearance.
    FSDK_Control_GetWidget(btn_submit, &widget);
    FSDK_Widget_SetHighlightingMode(widget, e_FSHighlightingPush);
    FSDK_Widget_SetMKBorderColor(widget, 0xFF0000);
    FSDK_Widget_SetMKBackgroundColor(widget, 0xF0F0F0);
    FSDK_Widget_SetMKNormalCaption(widget, L"Submit");

    FSDK_Annot_ResetAppearanceStream(widget, &return_value);

    // Set submit form action.
    FSDK_Form_GetDocument(form, &doc);
    FSDK_Action_Create(doc, e_FSTypeSubmitForm, &action_handle);
    FSDK_SubmitFormAction_Create(action_handle, &submit_action);

    FSDK_Form_GetFieldCount(form, NULL, &count);
    name_array = (FS_WSTR*)malloc(sizeof(FS_WSTR) * count);
    for (i = 0; i < count; i++)
      FSDK_WStr_Init(&name_array[i]);
    for (i = 0; i < count; i++) {
      FS_FIELD_HANDLE field = NULL;
      FSDK_Form_GetField(form, i, NULL, &field);
      FSDK_Field_GetName(field, &name_array[i]);
      FSDK_Field_Release(field);
    }

    FSDK_SubmitFormAction_SetFieldNames(submit_action, name_array, count);
    FSDK_SubmitFormAction_SetURL(submit_action, "http://www.foxitsoftware.com");
    FSDK_Widget_SetAction(widget, submit_action);
    FSDK_Action_Release(action_handle);
    free(name_array);
    printf("Add button field.\n");
  }
  {
    // Add radio button group
    FS_CONTROL_HANDLE control = NULL;
    FSRectF rect;
    FS_CONTROL_HANDLE control1 = NULL;
    FS_WIDGETANNOT_HANDLE widget = NULL;
    FS_BOOL result;
    FS_WIDGETANNOT_HANDLE widget1 = NULL;

    rect.left = 50.0;
    rect.bottom = 700;
    rect.right = 90;
    rect.top = 740;
    FSDK_Form_AddControl(form, page, L"Radio Button0", e_FSTypeRadioButton, rect, &control);

    rect.left = 100.0;
    rect.bottom = 700;
    rect.right = 140;
    rect.top = 740;
    FSDK_Form_AddControl(form, page, L"Radio Button0", e_FSTypeRadioButton, rect, &control1);
    FSDK_Control_SetExportValue(control, L"YES");
    FSDK_Control_SetChecked(control, TRUE);
    // Update radio button's appearance.

    FSDK_Control_GetWidget(control, &widget);
    FSDK_Annot_ResetAppearanceStream(widget, &result);

    FSDK_Control_SetExportValue(control1, L"NO");
    FSDK_Control_SetChecked(control1, FALSE);
    // Update radio button's appearance.
    FSDK_Control_GetWidget(control, &widget1);
    FSDK_Annot_ResetAppearanceStream(widget1, &result);

    FSDK_Widget_Release(widget);
    FSDK_Widget_Release(widget1);
    FSDK_Control_Release(control1);
    FSDK_Control_Release(control);
    printf("Add radio button.\n");
  }
  {
    // Add check box
    FS_CONTROL_HANDLE control = NULL;
    FS_WIDGETANNOT_HANDLE widget = NULL;
    FS_BOOL result;
    FSRectF rect;
    rect.left = 50.0;
    rect.bottom = 650;
    rect.right = 90;
    rect.top = 690;
    FSDK_Form_AddControl(form, page, L"Check Box0", e_FSTypeCheckBox, rect, &control);
    FSDK_Control_SetChecked(control, TRUE);

    FSDK_Control_GetWidget(control, &widget);
    FSDK_Widget_SetMKBorderColor(widget, 0x000000);
    FSDK_Widget_SetMKBackgroundColor(widget, 0xFFFFFF);
    FSDK_Annot_ResetAppearanceStream(widget, &result);
    FSDK_Widget_Release(widget);
    FSDK_Control_Release(control);
    printf("Add check box.\n");
  }
  {
    // Add text field
    FS_CONTROL_HANDLE control = NULL;
    FSRectF rect;
    FS_FIELD_HANDLE field = NULL;
    FS_BOOL result;
    FS_CONTROL_HANDLE control1 = NULL;
    FS_FIELD_HANDLE field1 = NULL;
    FS_WIDGETANNOT_HANDLE widget1 = NULL;
    FS_CONTROL_HANDLE control2 = NULL;
    FS_JAVASCRIPTACTION_HANDLE javascipt_action = NULL;
    FS_PDFDOC_HANDLE doc = NULL;
    FS_FIELD_HANDLE field2 = NULL;
    FS_ADDITIONALACTION_HANDLE additional_act = NULL;
    FS_WIDGETANNOT_HANDLE widget2 = NULL;
    FS_WIDGETANNOT_HANDLE widget = NULL;

    rect.left = 50;
    rect.bottom = 600;
    rect.right = 90;
    rect.top = 640;
    FSDK_Form_AddControl(form, page, L"Text Field0", e_FSTypeTextField, rect, &control);

    FSDK_Control_GetField(control, &field);
    FSDK_Field_SetValue(field, L"3");
    // Update text field's appearance.

    FSDK_Control_GetWidget(control, &widget);
    FSDK_Annot_ResetAppearanceStream(widget, &result);
    FSDK_Widget_Release(widget);
    FSDK_Control_Release(control);

    rect.left = 100;
    rect.bottom = 600;
    rect.right = 140;
    rect.top = 640;
    FSDK_Form_AddControl(form, page, L"Text Field1", e_FSTypeTextField, rect, &control1);

    FSDK_Control_GetField(control1, &field1);
    FSDK_Field_SetValue(field1, L"23");
    // Update text field's appearance.
    FSDK_Control_GetWidget(control1, &widget1);
    FSDK_Annot_ResetAppearanceStream(widget1, &result);

    rect.left = 150;
    rect.bottom = 600;
    rect.right = 190;
    rect.top = 640;
    FSDK_Form_AddControl(form, page, L"Text Field2", e_FSTypeTextField, rect, &control2);

    FSDK_Form_GetDocument(form, &doc);
    FSDK_Action_Create(doc, e_FSTypeJavaScript, &javascipt_action);
    FSDK_JavaScriptAction_SetScript(javascipt_action, L"AFSimple_Calculate(\"SUM\", new Array (\"Text Field0\", \"Text Field1\"));");

    FSDK_Control_GetField(control2, &field2);

    FSDK_AdditionalAction_Create1(field2, &additional_act);
    FSDK_AdditionalAction_SetAction(additional_act, e_FSTriggerFieldRecalculateValue, javascipt_action);
    FSDK_Field_SetValue(field2, L"333333");
    // Update text field's appearance.
    FSDK_Control_GetWidget(control2, &widget2);
    FSDK_Annot_ResetAppearanceStream(widget2, &result);

    FSDK_Widget_Release(widget1);
    FSDK_Control_Release(control1);
    FSDK_Widget_Release(widget2);
    FSDK_Control_Release(control2);
    printf("Add text field.\n");
  }
  {
    // Add text field with flag comb
    FS_CONTROL_HANDLE control = NULL;
    FSRectF rect;
    FS_FIELD_HANDLE field = NULL;
    FS_UINT32 flag = e_FSFieldFlagsFlagTextComb;
    FS_BOOL result;
    FS_WIDGETANNOT_HANDLE widget = NULL;

    rect.left = 50;
    rect.bottom = 570;
    rect.right = 100;
    rect.top = 590;
    FSDK_Form_AddControl(form, page, L"Text Field3", e_FSTypeTextField, rect, &control);

    FSDK_Control_GetField(control, &field);
    FSDK_Field_SetFlags(field, flag);
    FSDK_Field_SetValue(field, L"94538");
    FSDK_Field_SetMaxLength(field, 5);
    // Update text field's appearance.
    FSDK_Control_GetWidget(control, &widget);

    FSDK_Annot_ResetAppearanceStream(widget, &result);
    FSDK_Widget_Release(widget);
    FSDK_Control_Release(control);
  }
  {
    // Add text field, with flag multiline.
    FS_CONTROL_HANDLE control = NULL;
    FSRectF rect;
    FS_UINT32 flag = e_FSFieldFlagsFlagTextMultiline;
    FS_WIDGETANNOT_HANDLE widget = NULL;
    FS_BOOL result;

    FS_FIELD_HANDLE field = NULL;
    rect.left = 110;
    rect.bottom = 550;
    rect.right = 190;
    rect.top = 590;
    FSDK_Form_AddControl(form, page, L"Text Field4", e_FSTypeTextField, rect, &control);

    FSDK_Control_GetField(control, &field);

    FSDK_Field_SetFlags(field, flag);
    FSDK_Field_SetValue(field, L"Text fields are boxes or spaces in which the user can enter text from the keyboard.");
    // Update text field's appearance.
    FSDK_Control_GetWidget(control, &widget);

    FSDK_Annot_ResetAppearanceStream(widget, &result);
    FSDK_Widget_Release(widget);
    FSDK_Control_Release(control);
  }
  {
    // Add text field, with flag password.
    FS_CONTROL_HANDLE control = NULL;
    FSRectF rect;
    FS_FIELD_HANDLE field = NULL;
    FS_UINT32 flag = e_FSFieldFlagsFlagTextPassword;
    FS_WIDGETANNOT_HANDLE widget = NULL;
    FS_BOOL result;

    rect.left = 200;
    rect.bottom = 570;
    rect.right = 240;
    rect.top = 590;
    FSDK_Form_AddControl(form, page, L"Text Field5", e_FSTypeTextField, rect, &control);

    FSDK_Control_GetField(control, &field);

    FSDK_Field_SetFlags(field, flag);
    FSDK_Field_SetValue(field, L"Password");
    // Update text field's appearance.
    FSDK_Control_GetWidget(control, &widget);

    FSDK_Annot_ResetAppearanceStream(widget, &result);
    FSDK_Widget_Release(widget);
    FSDK_Control_Release(control);
  }
  {
    // Add list box
    FS_CONTROL_HANDLE control = NULL;
    FSRectF rect;
    FS_FIELD_HANDLE field = NULL;
    FSChoiceOption options[3];
    FS_WIDGETANNOT_HANDLE widget = NULL;
    FS_BOOL result;

    rect.left = 50;
    rect.bottom = 450;
    rect.right = 350;
    rect.top = 500;
    FSDK_Form_AddControl(form, page, L"List Box0", e_FSTypeListBox, rect, &control);

    FSDK_Control_GetField(control, &field);

    options[0].option_value.str = L"Foxit SDK";
    options[0].option_label.str = L"Foxit SDK";
    options[0].selected = TRUE;
    options[0].default_selected = TRUE;
    options[1].option_value.str = L"Foxit Reader";
    options[1].option_label.str = L"Foxit Reader";
    options[1].selected = TRUE;
    options[1].default_selected = TRUE;
    options[2].option_value.str = L"Foxit Phantom";
    options[2].option_label.str = L"Foxit Phantom";
    options[2].selected = TRUE;
    options[2].default_selected = TRUE;
    FSDK_Field_SetOptions(field, options, 3);

    FSDK_Control_GetWidget(control, &widget);
    FSDK_Widget_SetMKBorderColor(widget, 0x000000);
    FSDK_Widget_SetMKBackgroundColor(widget, 0xFFFFFF);

    FSDK_Annot_ResetAppearanceStream(widget, &result);
    FSDK_Widget_Release(widget);
    FSDK_Control_Release(control);
  }
  {
    // Add combo box
    FS_CONTROL_HANDLE control = NULL;
    FSRectF rect;
    FS_FIELD_HANDLE field = NULL;
    FSChoiceOption options[3];
    FS_WIDGETANNOT_HANDLE widget = NULL;
    FS_BOOL result;

    rect.left = 50;
    rect.bottom = 350;
    rect.right = 350;
    rect.top = 400;
    FSDK_Form_AddControl(form, page, L"Combo Box0", e_FSTypeComboBox, rect, &control);

    FSDK_Control_GetField(control, &field);

    options[0].option_value.str = L"Foxit SDK";
    options[0].option_label.str = L"Foxit SDK";
    options[0].selected = TRUE;
    options[0].default_selected = TRUE;
    options[1].option_value.str = L"Foxit Reader";
    options[1].option_label.str = L"Foxit Reader";
    options[1].selected = TRUE;
    options[1].default_selected = TRUE;
    options[2].option_value.str = L"Foxit Phantom";
    options[2].option_label.str = L"Foxit Phantom";
    options[2].selected = TRUE;
    options[2].default_selected = TRUE;
    FSDK_Field_SetOptions(field, options, 3);

    FSDK_Control_GetWidget(control, &widget);
    FSDK_Widget_SetMKBorderColor(widget, 0x000000);
    FSDK_Widget_SetMKBackgroundColor(widget, 0xFFFFFF);

    FSDK_Annot_ResetAppearanceStream(widget, &result);
    FSDK_Widget_Release(widget);
    FSDK_Control_Release(control);
  }
}
int main(int argc, char *argv[]) {
  int err_ret = 0;
  FSErrorCode error_code = e_FSErrSuccess;
  wchar_t output_directory[MAX_FILE_PATH];

  error_code = FSDK_Library_Initialize(sn, key);
  if (error_code != e_FSErrSuccess) {
    printf("Library Initialize Error: %d\n", error_code);
    return 1;
  }

  swprintf_s(output_directory, MAX_FILE_PATH, L"%lsform/", output_path);
  _wmkdir(output_directory);

  {
    FS_PDFDOC_HANDLE doc = NULL;
    FS_FORM_HANDLE form = NULL;
    FS_PDFPAGE_HANDLE page = NULL;
    wchar_t new_pdf[MAX_FILE_PATH];
    FS_BOOL is_save;

    FSDK_PDFDoc_Create(&doc);

    FSDK_Form_Create(doc, &form);
    // Create a blank new page and add some form fields.
    FSDK_PDFDoc_InsertPage0(doc, 0, e_FSSizeLetter, &page);
    AddInteractiveForms(page, form);

    swprintf_s(new_pdf, MAX_FILE_PATH, L"%lsform.pdf", output_directory);

    FSDK_PDFDoc_SaveAs(doc, new_pdf, e_FSSaveFlagNoOriginal, &is_save);
    FSDK_PDFPage_Release(page);
    FSDK_Form_Release(form);
    FSDK_PDFDoc_Release(doc);
  }
  FSDK_Library_Release();
  return err_ret;
}
