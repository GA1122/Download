void AddImportDataStrings(content::WebUIDataSource* html_source) {
  LocalizedString localized_strings[] = {
    {"importTitle", IDS_SETTINGS_IMPORT_SETTINGS_TITLE},
    {"importFromLabel", IDS_SETTINGS_IMPORT_FROM_LABEL},
    {"importDescription", IDS_SETTINGS_IMPORT_ITEMS_LABEL},
    {"importLoading", IDS_SETTINGS_IMPORT_LOADING_PROFILES},
    {"importHistory", IDS_SETTINGS_IMPORT_HISTORY_CHECKBOX},
    {"importFavorites", IDS_SETTINGS_IMPORT_FAVORITES_CHECKBOX},
    {"importPasswords", IDS_SETTINGS_IMPORT_PASSWORDS_CHECKBOX},
    {"importSearch", IDS_SETTINGS_IMPORT_SEARCH_ENGINES_CHECKBOX},
    {"importAutofillFormData", IDS_SETTINGS_IMPORT_AUTOFILL_FORM_DATA_CHECKBOX},
    {"importChooseFile", IDS_SETTINGS_IMPORT_CHOOSE_FILE},
    {"importCommit", IDS_SETTINGS_IMPORT_COMMIT},
    {"noProfileFound", IDS_SETTINGS_IMPORT_NO_PROFILE_FOUND},
    {"importSuccess", IDS_SETTINGS_IMPORT_SUCCESS},
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));
}
