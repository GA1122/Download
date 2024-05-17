StartSyncArgs::StartSyncArgs(
    Profile* profile,
    Browser* browser,
    OneClickSigninHelper::AutoAccept auto_accept,
    const std::string& session_index,
    const std::string& email,
    const std::string& password,
    bool force_same_tab_navigation,
    bool untrusted_confirmation_required,
    SyncPromoUI::Source source)
    : profile(profile),
      browser(browser),
      auto_accept(auto_accept),
      session_index(session_index),
      email(email),
      password(password),
      force_same_tab_navigation(force_same_tab_navigation),
      source(source) {
  if (untrusted_confirmation_required) {
    confirmation_required = OneClickSigninSyncStarter::CONFIRM_UNTRUSTED_SIGNIN;
  } else if (source == SyncPromoUI::SOURCE_SETTINGS ||
             source == SyncPromoUI::SOURCE_WEBSTORE_INSTALL) {
    confirmation_required = OneClickSigninSyncStarter::NO_CONFIRMATION;
  } else {
    confirmation_required = OneClickSigninSyncStarter::CONFIRM_AFTER_SIGNIN;
  }
}
