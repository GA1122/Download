void AddCommonStrings(content::WebUIDataSource* html_source, Profile* profile) {
  LocalizedString localized_strings[] = {
    {"add", IDS_ADD},
    {"advancedPageTitle", IDS_SETTINGS_ADVANCED},
    {"back", IDS_ACCNAME_BACK},
    {"basicPageTitle", IDS_SETTINGS_BASIC},
    {"cancel", IDS_CANCEL},
    {"clear", IDS_SETTINGS_CLEAR},
    {"close", IDS_CLOSE},
    {"confirm", IDS_CONFIRM},
    {"controlledByExtension", IDS_SETTINGS_CONTROLLED_BY_EXTENSION},
#if defined(OS_CHROMEOS)
    {"deviceOff", IDS_SETTINGS_DEVICE_OFF},
    {"deviceOn", IDS_SETTINGS_DEVICE_ON},
#endif
    {"disable", IDS_DISABLE},
    {"done", IDS_DONE},
    {"edit", IDS_SETTINGS_EDIT},
    {"learnMore", IDS_LEARN_MORE},
    {"menuButtonLabel", IDS_SETTINGS_MENU_BUTTON_LABEL},
    {"moreActions", IDS_SETTINGS_MORE_ACTIONS},
    {"ok", IDS_OK},
    {"restart", IDS_SETTINGS_RESTART},
    {"save", IDS_SAVE},
    {"settings", IDS_SETTINGS_SETTINGS},
    {"toggleOn", IDS_SETTINGS_TOGGLE_ON},
    {"toggleOff", IDS_SETTINGS_TOGGLE_OFF},
    {"notValid", IDS_SETTINGS_NOT_VALID},
    {"notValidWebAddress", IDS_SETTINGS_NOT_VALID_WEB_ADDRESS},
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));

  html_source->AddBoolean(
      "isGuest",
#if defined(OS_CHROMEOS)
      user_manager::UserManager::Get()->IsLoggedInAsGuest() ||
      user_manager::UserManager::Get()->IsLoggedInAsPublicAccount());
#else
      profile->IsOffTheRecord());
#endif

  html_source->AddBoolean("isSupervised", profile->IsSupervised());
}
