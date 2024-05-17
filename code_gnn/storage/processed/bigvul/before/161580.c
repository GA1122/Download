void AddLanguagesStrings(content::WebUIDataSource* html_source) {
  LocalizedString localized_strings[] = {
    {"languagesPageTitle", IDS_SETTINGS_LANGUAGES_PAGE_TITLE},
    {"languagesListTitle", IDS_SETTINGS_LANGUAGES_LANGUAGES_LIST_TITLE},
    {"languagesExpandA11yLabel",
     IDS_SETTINGS_LANGUAGES_EXPAND_ACCESSIBILITY_LABEL},
    {"orderLanguagesInstructions",
     IDS_SETTINGS_LANGUAGES_LANGUAGES_LIST_ORDERING_INSTRUCTIONS},
    {"moveToTop", IDS_SETTINGS_LANGUAGES_LANGUAGES_LIST_MOVE_TO_TOP},
    {"moveUp", IDS_SETTINGS_LANGUAGES_LANGUAGES_LIST_MOVE_UP},
    {"moveDown", IDS_SETTINGS_LANGUAGES_LANGUAGES_LIST_MOVE_DOWN},
    {"removeLanguage", IDS_SETTINGS_LANGUAGES_LANGUAGES_LIST_REMOVE},
    {"addLanguages", IDS_SETTINGS_LANGUAGES_LANGUAGES_ADD},
#if defined(OS_CHROMEOS)
    {"inputMethodsListTitle", IDS_SETTINGS_LANGUAGES_INPUT_METHODS_LIST_TITLE},
    {"inputMethodEnabled", IDS_SETTINGS_LANGUAGES_INPUT_METHOD_ENABLED},
    {"inputMethodsExpandA11yLabel",
     IDS_SETTINGS_LANGUAGES_INPUT_METHODS_EXPAND_ACCESSIBILITY_LABEL},
    {"manageInputMethods", IDS_SETTINGS_LANGUAGES_INPUT_METHODS_MANAGE},
    {"manageInputMethodsPageTitle",
     IDS_SETTINGS_LANGUAGES_MANAGE_INPUT_METHODS_TITLE},
    {"showImeMenu", IDS_SETTINGS_LANGUAGES_SHOW_IME_MENU},
#endif
    {"addLanguagesDialogTitle", IDS_SETTINGS_LANGUAGES_MANAGE_LANGUAGES_TITLE},
    {"allLanguages", IDS_SETTINGS_LANGUAGES_ALL_LANGUAGES},
    {"enabledLanguages", IDS_SETTINGS_LANGUAGES_ENABLED_LANGUAGES},
    {"isDisplayedInThisLanguage",
     IDS_SETTINGS_LANGUAGES_IS_DISPLAYED_IN_THIS_LANGUAGE},
    {"displayInThisLanguage", IDS_SETTINGS_LANGUAGES_DISPLAY_IN_THIS_LANGUAGE},
    {"offerToTranslateInThisLanguage",
     IDS_SETTINGS_LANGUAGES_OFFER_TO_TRANSLATE_IN_THIS_LANGUAGE},
    {"offerToEnableTranslate",
     IDS_SETTINGS_LANGUAGES_OFFER_TO_ENABLE_TRANSLATE},
#if !defined(OS_MACOSX)
    {"spellCheckListTitle", IDS_SETTINGS_LANGUAGES_SPELL_CHECK_LIST_TITLE},
    {"spellCheckExpandA11yLabel",
     IDS_SETTINGS_LANGUAGES_SPELL_CHECK_EXPAND_ACCESSIBILITY_LABEL},
    {"spellCheckSummaryTwoLanguages",
     IDS_SETTINGS_LANGUAGES_SPELL_CHECK_SUMMARY_TWO_LANGUAGES},
    {"spellCheckSummaryThreeLanguages",
     IDS_SETTINGS_LANGUAGES_SPELL_CHECK_SUMMARY_THREE_LANGUAGES},
    {"spellCheckSummaryMultipleLanguages",
     IDS_SETTINGS_LANGUAGES_SPELL_CHECK_SUMMARY_MULTIPLE_LANGUAGES},
    {"manageSpellCheck", IDS_SETTINGS_LANGUAGES_SPELL_CHECK_MANAGE},
    {"editDictionaryPageTitle", IDS_SETTINGS_LANGUAGES_EDIT_DICTIONARY_TITLE},
    {"addDictionaryWordLabel", IDS_SETTINGS_LANGUAGES_ADD_DICTIONARY_WORD},
    {"addDictionaryWordButton",
     IDS_SETTINGS_LANGUAGES_ADD_DICTIONARY_WORD_BUTTON},
    {"addDictionaryWordDuplicateError",
     IDS_SETTINGS_LANGUAGES_ADD_DICTIONARY_WORD_DUPLICATE_ERROR},
    {"addDictionaryWordLengthError",
     IDS_SETTINGS_LANGUAGES_ADD_DICTIONARY_WORD_LENGTH_ERROR},
    {"customDictionaryWords", IDS_SETTINGS_LANGUAGES_DICTIONARY_WORDS},
    {"noCustomDictionaryWordsFound",
     IDS_SETTINGS_LANGUAGES_DICTIONARY_WORDS_NONE},
    {"spellCheckDisabled", IDS_SETTINGS_LANGUAGES_SPELL_CHECK_DISABLED},
#endif
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

#if defined(OS_CHROMEOS)
  html_source->AddString(
      "languagesLearnMoreURL",
      base::ASCIIToUTF16(chrome::kLanguageSettingsLearnMoreUrl));
#endif
}
