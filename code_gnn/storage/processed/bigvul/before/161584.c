void AddPeopleStrings(content::WebUIDataSource* html_source, Profile* profile) {
  LocalizedString localized_strings[] = {
    {"peoplePageTitle", IDS_SETTINGS_PEOPLE},
    {"manageOtherPeople", IDS_SETTINGS_PEOPLE_MANAGE_OTHER_PEOPLE},
#if defined(OS_CHROMEOS)
    {"configureFingerprintTitle", IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_TITLE},
    {"configureFingerprintInstructionLocateScannerStep",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_INSTRUCTION_LOCATE_SCANNER},
    {"configureFingerprintInstructionMoveFingerStep",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_INSTRUCTION_MOVE_FINGER},
    {"configureFingerprintInstructionReadyStep",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_INSTRUCTION_READY},
    {"configureFingerprintLiftFinger",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_LIFT_FINGER},
    {"configureFingerprintPartialData",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_PARTIAL_DATA},
    {"configureFingerprintInsufficientData",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_INSUFFICIENT_DATA},
    {"configureFingerprintSensorDirty",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_SENSOR_DIRTY},
    {"configureFingerprintTooSlow",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_FINGER_TOO_SLOW},
    {"configureFingerprintTooFast",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_FINGER_TOO_FAST},
    {"configureFingerprintImmobile",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_FINGER_IMMOBILE},
    {"configureFingerprintAddAnotherButton",
     IDS_SETTINGS_ADD_FINGERPRINT_DIALOG_ADD_ANOTHER_BUTTON},
    {"configurePinChoosePinTitle",
     IDS_SETTINGS_PEOPLE_CONFIGURE_PIN_CHOOSE_PIN_TITLE},
    {"configurePinConfirmPinTitle",
     IDS_SETTINGS_PEOPLE_CONFIGURE_PIN_CONFIRM_PIN_TITLE},
    {"configurePinContinueButton",
     IDS_SETTINGS_PEOPLE_CONFIGURE_PIN_CONTINUE_BUTTON},
    {"configurePinMismatched", IDS_SETTINGS_PEOPLE_CONFIGURE_PIN_MISMATCHED},
    {"configurePinTooShort", IDS_SETTINGS_PEOPLE_CONFIGURE_PIN_TOO_SHORT},
    {"configurePinTooLong", IDS_SETTINGS_PEOPLE_CONFIGURE_PIN_TOO_LONG},
    {"configurePinWeakPin", IDS_SETTINGS_PEOPLE_CONFIGURE_PIN_WEAK_PIN},
    {"enableScreenlock", IDS_SETTINGS_PEOPLE_ENABLE_SCREENLOCK},
    {"lockScreenAddFingerprint",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_ADD_FINGERPRINT_BUTTON},
    {"lockScreenChangePinButton",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_CHANGE_PIN_BUTTON},
    {"lockScreenEditFingerprints",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_EDIT_FINGERPRINTS},
    {"lockScreenEditFingerprintsDescription",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_EDIT_FINGERPRINTS_DESCRIPTION},
    {"lockScreenSetupFingerprintButton",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_FINGERPRINT_SETUP_BUTTON},
    {"lockScreenNumberFingerprints",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_NUM_FINGERPRINTS},
    {"lockScreenNone", IDS_SETTINGS_PEOPLE_LOCK_SCREEN_NONE},
    {"lockScreenFingerprintEnable",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_ENABLE_FINGERPRINT_CHECKBOX_LABEL},
    {"lockScreenFingerprintNewName",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_NEW_FINGERPRINT_DEFAULT_NAME},
    {"lockScreenFingerprintTitle",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_FINGERPRINT_SUBPAGE_TITLE},
    {"lockScreenFingerprintWarning",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_FINGERPRINT_LESS_SECURE},
    {"lockScreenOptions", IDS_SETTINGS_PEOPLE_LOCK_SCREEN_OPTIONS},
    {"lockScreenPasswordOnly", IDS_SETTINGS_PEOPLE_LOCK_SCREEN_PASSWORD_ONLY},
    {"lockScreenPinOrPassword",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_PIN_OR_PASSWORD},
    {"lockScreenRegisteredFingerprints",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_REGISTERED_FINGERPRINTS_LABEL},
    {"lockScreenSetupPinButton",
     IDS_SETTINGS_PEOPLE_LOCK_SCREEN_SETUP_PIN_BUTTON},
    {"lockScreenTitle", IDS_SETTINGS_PEOPLE_LOCK_SCREEN_TITLE},
    {"passwordPromptEnterPassword",
     IDS_SETTINGS_PEOPLE_PASSWORD_PROMPT_ENTER_PASSWORD},
    {"passwordPromptInvalidPassword",
     IDS_SETTINGS_PEOPLE_PASSWORD_PROMPT_INVALID_PASSWORD},
    {"passwordPromptPasswordLabel",
     IDS_SETTINGS_PEOPLE_PASSWORD_PROMPT_PASSWORD_LABEL},
    {"passwordPromptTitle", IDS_SETTINGS_PEOPLE_PASSWORD_PROMPT_TITLE},
    {"pinKeyboardPlaceholderPin", IDS_PIN_KEYBOARD_HINT_TEXT_PIN},
    {"pinKeyboardPlaceholderPinPassword",
     IDS_PIN_KEYBOARD_HINT_TEXT_PIN_PASSWORD},
    {"pinKeyboardDeleteAccessibleName",
     IDS_PIN_KEYBOARD_DELETE_ACCESSIBLE_NAME},
    {"changePictureTitle", IDS_SETTINGS_CHANGE_PICTURE_DIALOG_TITLE},
    {"changePicturePageDescription", IDS_SETTINGS_CHANGE_PICTURE_DIALOG_TEXT},
    {"takePhoto", IDS_SETTINGS_CHANGE_PICTURE_TAKE_PHOTO},
    {"captureVideo", IDS_SETTINGS_CHANGE_PICTURE_CAPTURE_VIDEO},
    {"discardPhoto", IDS_SETTINGS_CHANGE_PICTURE_DISCARD_PHOTO},
    {"switchModeToCamera", IDS_SETTINGS_CHANGE_PICTURE_SWITCH_MODE_TO_CAMERA},
    {"switchModeToVideo", IDS_SETTINGS_CHANGE_PICTURE_SWITCH_MODE_TO_VIDEO},
    {"chooseFile", IDS_SETTINGS_CHANGE_PICTURE_CHOOSE_FILE},
    {"profilePhoto", IDS_SETTINGS_CHANGE_PICTURE_PROFILE_PHOTO},
    {"oldPhoto", IDS_SETTINGS_CHANGE_PICTURE_OLD_PHOTO},
    {"previewAltText", IDS_SETTINGS_CHANGE_PICTURE_PREVIEW_ALT},
    {"authorCreditText", IDS_SETTINGS_CHANGE_PICTURE_AUTHOR_CREDIT_TEXT},
    {"photoCaptureAccessibleText", IDS_SETTINGS_PHOTO_CAPTURE_ACCESSIBLE_TEXT},
    {"photoDiscardAccessibleText", IDS_SETTINGS_PHOTO_DISCARD_ACCESSIBLE_TEXT},
    {"photoModeAccessibleText", IDS_SETTINGS_PHOTO_MODE_ACCESSIBLE_TEXT},
    {"videoModeAccessibleText", IDS_SETTINGS_VIDEO_MODE_ACCESSIBLE_TEXT},
#else    
    {"domainManagedProfile", IDS_SETTINGS_PEOPLE_DOMAIN_MANAGED_PROFILE},
    {"editPerson", IDS_SETTINGS_EDIT_PERSON},
    {"profileNameAndPicture", IDS_SETTINGS_PROFILE_NAME_AND_PICTURE},
    {"showShortcutLabel", IDS_SETTINGS_PROFILE_SHORTCUT_TOGGLE_LABEL},
    {"syncWillStart", IDS_SETTINGS_SYNC_WILL_START},
    {"syncSettingsSavedToast", IDS_SETTINGS_SYNC_SETTINGS_SAVED_TOAST_LABEL},
    {"cancelSync", IDS_SETTINGS_SYNC_SETTINGS_CANCEL_SYNC},
#endif   
#if BUILDFLAG(ENABLE_DICE_SUPPORT)
    {"peopleSignIn", IDS_SETTINGS_PEOPLE_SIGN_IN},
    {"peopleSignInPrompt", IDS_SETTINGS_PEOPLE_SIGN_IN_PROMPT},
    {"peopleSignInPromptSecondary",
     IDS_SETTINGS_PEOPLE_SIGN_IN_PROMPT_SECONDARY},
    {"useAnotherAccount", IDS_SETTINGS_PEOPLE_SYNC_ANOTHER_ACCOUNT},
    {"syncAsName", IDS_SETTINGS_PEOPLE_SYNC_AS_NAME},
    {"syncedToName", IDS_SETTINGS_PEOPLE_SYNCED_AS_NAME},
    {"turnOffSync", IDS_SETTINGS_PEOPLE_SYNC_TURN_OFF},
    {"syncNotWorking", IDS_SETTINGS_PEOPLE_SYNC_NOT_WORKING},
#endif
    {"syncOverview", IDS_SETTINGS_SYNC_OVERVIEW},
    {"syncDisabledByAdministrator",
     IDS_SETTINGS_SYNC_DISABLED_BY_ADMINISTRATOR},
    {"syncSignin", IDS_SETTINGS_SYNC_SIGNIN},
    {"syncDisconnect", IDS_SETTINGS_SYNC_DISCONNECT},
    {"syncDisconnectTitle", IDS_SETTINGS_SYNC_DISCONNECT_TITLE},
    {"syncDisconnectDeleteProfile",
     IDS_SETTINGS_SYNC_DISCONNECT_DELETE_PROFILE},
    {"deleteProfileWarningExpandA11yLabel",
     IDS_SETTINGS_SYNC_DISCONNECT_EXPAND_ACCESSIBILITY_LABEL},
    {"deleteProfileWarningWithCountsSingular",
     IDS_SETTINGS_SYNC_DISCONNECT_DELETE_PROFILE_WARNING_WITH_COUNTS_SINGULAR},
    {"deleteProfileWarningWithCountsPlural",
     IDS_SETTINGS_SYNC_DISCONNECT_DELETE_PROFILE_WARNING_WITH_COUNTS_PLURAL},
    {"deleteProfileWarningWithoutCounts",
     IDS_SETTINGS_SYNC_DISCONNECT_DELETE_PROFILE_WARNING_WITHOUT_COUNTS},
    {"syncDisconnectExplanation", IDS_SETTINGS_SYNC_DISCONNECT_EXPLANATION},
    {"syncDisconnectConfirm", IDS_SETTINGS_SYNC_DISCONNECT_CONFIRM},
    {"sync", IDS_SETTINGS_SYNC},
    {"syncPageTitle", IDS_SETTINGS_SYNC_PAGE_TITLE},
    {"syncLoading", IDS_SETTINGS_SYNC_LOADING},
    {"syncTimeout", IDS_SETTINGS_SYNC_TIMEOUT},
    {"syncEverythingCheckboxLabel",
     IDS_SETTINGS_SYNC_EVERYTHING_CHECKBOX_LABEL},
    {"appCheckboxLabel", IDS_SETTINGS_APPS_CHECKBOX_LABEL},
    {"extensionsCheckboxLabel", IDS_SETTINGS_EXTENSIONS_CHECKBOX_LABEL},
    {"settingsCheckboxLabel", IDS_SETTINGS_SETTINGS_CHECKBOX_LABEL},
    {"autofillCheckboxLabel", IDS_SETTINGS_AUTOFILL_CHECKBOX_LABEL},
    {"historyCheckboxLabel", IDS_SETTINGS_HISTORY_CHECKBOX_LABEL},
    {"themesAndWallpapersCheckboxLabel",
     IDS_SETTINGS_THEMES_AND_WALLPAPERS_CHECKBOX_LABEL},
    {"bookmarksCheckboxLabel", IDS_SETTINGS_BOOKMARKS_CHECKBOX_LABEL},
    {"passwordsCheckboxLabel", IDS_SETTINGS_PASSWORDS_CHECKBOX_LABEL},
    {"openTabsCheckboxLabel", IDS_SETTINGS_OPEN_TABS_CHECKBOX_LABEL},
    {"enablePaymentsIntegrationCheckboxLabel",
     IDS_SETTINGS_ENABLE_PAYMENTS_INTEGRATION_CHECKBOX_LABEL},
    {"manageSyncedDataTitle", IDS_SETTINGS_MANAGE_SYNCED_DATA_TITLE},
    {"encryptionOptionsTitle", IDS_SETTINGS_ENCRYPTION_OPTIONS},
    {"syncDataEncryptedText", IDS_SETTINGS_SYNC_DATA_ENCRYPTED_TEXT},
    {"encryptWithGoogleCredentialsLabel",
     IDS_SETTINGS_ENCRYPT_WITH_GOOGLE_CREDENTIALS_LABEL},
    {"useDefaultSettingsButton", IDS_SETTINGS_USE_DEFAULT_SETTINGS},
    {"emptyPassphraseError", IDS_SETTINGS_EMPTY_PASSPHRASE_ERROR},
    {"mismatchedPassphraseError", IDS_SETTINGS_MISMATCHED_PASSPHRASE_ERROR},
    {"incorrectPassphraseError", IDS_SETTINGS_INCORRECT_PASSPHRASE_ERROR},
    {"passphrasePlaceholder", IDS_SETTINGS_PASSPHRASE_PLACEHOLDER},
    {"passphraseConfirmationPlaceholder",
     IDS_SETTINGS_PASSPHRASE_CONFIRMATION_PLACEHOLDER},
    {"submitPassphraseButton", IDS_SETTINGS_SUBMIT_PASSPHRASE},
    {"personalizeGoogleServicesTitle",
     IDS_SETTINGS_PERSONALIZE_GOOGLE_SERVICES_TITLE},
    {"personalizeGoogleServicesText",
     IDS_SETTINGS_PERSONALIZE_GOOGLE_SERVICES_TEXT},
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

  for (int j = 0; j <= 9; j++) {
    html_source->AddString("pinKeyboard" + base::IntToString(j),
                           base::FormatNumber(int64_t{j}));
  }

  html_source->AddString("syncLearnMoreUrl", chrome::kSyncLearnMoreURL);
  html_source->AddString("autofillHelpURL",
#if defined(OS_CHROMEOS)
                         GetHelpUrlWithBoard(autofill::kHelpURL));
#else
                         autofill::kHelpURL);
#endif
  html_source->AddString("supervisedUsersUrl",
                         chrome::kLegacySupervisedUserManagementURL);

  html_source->AddString(
      "encryptWithSyncPassphraseLabel",
      l10n_util::GetStringFUTF8(
          IDS_SETTINGS_ENCRYPT_WITH_SYNC_PASSPHRASE_LABEL,
#if defined(OS_CHROMEOS)
          GetHelpUrlWithBoard(chrome::kSyncEncryptionHelpURL)));
#else
          base::ASCIIToUTF16(chrome::kSyncEncryptionHelpURL)));
#endif

  std::string sync_dashboard_url =
      google_util::AppendGoogleLocaleParam(
          GURL(chrome::kSyncGoogleDashboardURL),
          g_browser_process->GetApplicationLocale())
          .spec();
  html_source->AddString("syncDashboardUrl", sync_dashboard_url);

  html_source->AddString(
      "passphraseExplanationText",
      l10n_util::GetStringFUTF8(IDS_SETTINGS_PASSPHRASE_EXPLANATION_TEXT,
                                base::ASCIIToUTF16(sync_dashboard_url)));
  html_source->AddString(
      "passphraseResetHint",
      l10n_util::GetStringFUTF8(IDS_SETTINGS_PASSPHRASE_RESET_HINT,
                                base::ASCIIToUTF16(sync_dashboard_url)));
  html_source->AddString(
      "passphraseRecover",
      l10n_util::GetStringFUTF8(IDS_SETTINGS_PASSPHRASE_RECOVER,
                                base::ASCIIToUTF16(sync_dashboard_url)));
  html_source->AddString(
      "syncDisconnectExplanation",
      l10n_util::GetStringFUTF8(IDS_SETTINGS_SYNC_DISCONNECT_EXPLANATION,
                                base::ASCIIToUTF16(sync_dashboard_url)));
#if !defined(OS_CHROMEOS)
  html_source->AddString(
      "syncDisconnectManagedProfileExplanation",
      l10n_util::GetStringFUTF8(
          IDS_SETTINGS_SYNC_DISCONNECT_MANAGED_PROFILE_EXPLANATION,
          base::ASCIIToUTF16("$1"),
          base::ASCIIToUTF16(sync_dashboard_url)));

  if (signin::IsDiceEnabledForProfile(profile->GetPrefs())) {
    LocalizedString sync_disconnect_strings[] = {
        {"syncDisconnect", IDS_SETTINGS_TURN_OFF_SYNC_DIALOG_CONFIRM},
        {"syncDisconnectTitle", IDS_SETTINGS_TURN_OFF_SYNC_DIALOG_TITLE},
        {"syncDisconnectDeleteProfile",
         IDS_SETTINGS_TURN_OFF_SYNC_DIALOG_CHECKBOX},
        {"syncDisconnectConfirm",
         IDS_SETTINGS_TURN_OFF_SYNC_DIALOG_MANAGED_CONFIRM},
    };
    AddLocalizedStringsBulk(html_source, sync_disconnect_strings,
                            arraysize(sync_disconnect_strings));
  }
#endif

  html_source->AddString("syncErrorHelpUrl", chrome::kSyncErrorsHelpURL);

  html_source->AddString("activityControlsUrl",
                         chrome::kGoogleAccountActivityControlsURL);

  html_source->AddBoolean("profileShortcutsEnabled",
                          ProfileShortcutManager::IsFeatureEnabled());

  html_source->AddBoolean(
      "changePictureVideoModeEnabled",
      base::FeatureList::IsEnabled(features::kChangePictureVideoMode));
}
