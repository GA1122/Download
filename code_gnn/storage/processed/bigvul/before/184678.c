 void ExtensionSettingsHandler::GetLocalizedValues(
     DictionaryValue* localized_strings) {
   RegisterTitle(localized_strings, "extensionSettings",
                 IDS_MANAGE_EXTENSIONS_SETTING_WINDOWS_TITLE);
 
   localized_strings->SetString("extensionSettingsVisitWebsite",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_VISIT_WEBSITE));
 
   localized_strings->SetString("extensionSettingsDeveloperMode",
     l10n_util::GetStringUTF16(IDS_EXTENSIONS_DEVELOPER_MODE_LINK));
   localized_strings->SetString("extensionSettingsNoExtensions",
     l10n_util::GetStringUTF16(IDS_EXTENSIONS_NONE_INSTALLED));
   localized_strings->SetString("extensionSettingsSuggestGallery",
       l10n_util::GetStringFUTF16(IDS_EXTENSIONS_NONE_INSTALLED_SUGGEST_GALLERY,
           ASCIIToUTF16(google_util::AppendGoogleLocaleParam(
               GURL(extension_urls::GetWebstoreLaunchURL())).spec())));
   localized_strings->SetString("extensionSettingsGetMoreExtensions",
       l10n_util::GetStringFUTF16(IDS_GET_MORE_EXTENSIONS,
           ASCIIToUTF16(google_util::AppendGoogleLocaleParam(
               GURL(extension_urls::GetWebstoreLaunchURL())).spec())));
   localized_strings->SetString("extensionSettingsExtensionId",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_ID));
   localized_strings->SetString("extensionSettingsExtensionPath",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_PATH));
   localized_strings->SetString("extensionSettingsInspectViews",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_INSPECT_VIEWS));
   localized_strings->SetString("viewIncognito",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_VIEW_INCOGNITO));
   localized_strings->SetString("extensionSettingsEnable",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_ENABLE));
   localized_strings->SetString("extensionSettingsEnabled",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_ENABLED));
   localized_strings->SetString("extensionSettingsRemove",
     l10n_util::GetStringUTF16(IDS_EXTENSIONS_REMOVE));
   localized_strings->SetString("extensionSettingsEnableIncognito",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_ENABLE_INCOGNITO));
    localized_strings->SetString("extensionSettingsAllowFileAccess",
        l10n_util::GetStringUTF16(IDS_EXTENSIONS_ALLOW_FILE_ACCESS));
    localized_strings->SetString("extensionSettingsIncognitoWarning",
      l10n_util::GetStringFUTF16(IDS_EXTENSIONS_INCOGNITO_WARNING,
                                 l10n_util::GetStringUTF16(IDS_PRODUCT_NAME)));
//       l10n_util::GetStringUTF16(IDS_EXTENSIONS_INCOGNITO_WARNING));
    localized_strings->SetString("extensionSettingsReload",
        l10n_util::GetStringUTF16(IDS_EXTENSIONS_RELOAD));
    localized_strings->SetString("extensionSettingsOptions",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_OPTIONS));
   localized_strings->SetString("extensionSettingsPolicyControlled",
      l10n_util::GetStringUTF16(IDS_EXTENSIONS_POLICY_CONTROLLED));
   localized_strings->SetString("extensionSettingsShowButton",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_SHOW_BUTTON));
   localized_strings->SetString("extensionSettingsLoadUnpackedButton",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_LOAD_UNPACKED_BUTTON));
   localized_strings->SetString("extensionSettingsPackButton",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_PACK_BUTTON));
   localized_strings->SetString("extensionSettingsUpdateButton",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_UPDATE_BUTTON));
   localized_strings->SetString("extensionSettingsCrashMessage",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_CRASHED_EXTENSION));
   localized_strings->SetString("extensionSettingsInDevelopment",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_IN_DEVELOPMENT));
   localized_strings->SetString("extensionSettingsWarningsTitle",
       l10n_util::GetStringUTF16(IDS_EXTENSION_WARNINGS_TITLE));
   localized_strings->SetString("extensionSettingsShowDetails",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_SHOW_DETAILS));
   localized_strings->SetString("extensionSettingsHideDetails",
       l10n_util::GetStringUTF16(IDS_EXTENSIONS_HIDE_DETAILS));
 }