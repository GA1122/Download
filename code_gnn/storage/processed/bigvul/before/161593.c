void AddUsersStrings(content::WebUIDataSource* html_source) {
  LocalizedString localized_strings[] = {
    {"usersModifiedByOwnerLabel", IDS_SETTINGS_USERS_MODIFIED_BY_OWNER_LABEL},
    {"guestBrowsingLabel", IDS_SETTINGS_USERS_GUEST_BROWSING_LABEL},
    {"settingsManagedLabel", IDS_SETTINGS_USERS_MANAGED_LABEL},
    {"showOnSigninLabel", IDS_SETTINGS_USERS_SHOW_ON_SIGNIN_LABEL},
    {"restrictSigninLabel", IDS_SETTINGS_USERS_RESTRICT_SIGNIN_LABEL},
    {"deviceOwnerLabel", IDS_SETTINGS_USERS_DEVICE_OWNER_LABEL},
    {"addUsers", IDS_SETTINGS_USERS_ADD_USERS},
    {"addUsersEmail", IDS_SETTINGS_USERS_ADD_USERS_EMAIL},
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));
}
