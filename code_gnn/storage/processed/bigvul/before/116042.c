void NTPResourceCache::CreateNewTabIncognitoHTML() {
  DictionaryValue localized_strings;
  localized_strings.SetString("title",
      l10n_util::GetStringUTF16(IDS_NEW_TAB_TITLE));
  int new_tab_message_ids = IDS_NEW_TAB_OTR_MESSAGE;
  int new_tab_html_idr = IDR_INCOGNITO_TAB_HTML;
  const char* new_tab_link = kLearnMoreIncognitoUrl;
#if defined(OS_CHROMEOS)
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kGuestSession)) {
    new_tab_message_ids = IDS_NEW_TAB_GUEST_SESSION_MESSAGE;
    new_tab_html_idr = IDR_GUEST_SESSION_TAB_HTML;
    new_tab_link = kLearnMoreGuestSessionUrl;
  }
#endif
  localized_strings.SetString("content",
      l10n_util::GetStringFUTF16(new_tab_message_ids,
                                 GetUrlWithLang(GURL(new_tab_link))));
  localized_strings.SetString("extensionsmessage",
      l10n_util::GetStringFUTF16(
          IDS_NEW_TAB_OTR_EXTENSIONS_MESSAGE,
          l10n_util::GetStringUTF16(IDS_PRODUCT_NAME),
          ASCIIToUTF16(std::string(chrome::kChromeUISettingsURL) +
                       chrome::kExtensionsSubPage)));
  bool bookmark_bar_attached = profile_->GetPrefs()->GetBoolean(
      prefs::kShowBookmarkBar);
  localized_strings.SetString("bookmarkbarattached",
      bookmark_bar_attached ? "true" : "false");

  ChromeURLDataManager::DataSource::SetFontAndTextDirection(&localized_strings);

  static const base::StringPiece incognito_tab_html(
      ResourceBundle::GetSharedInstance().GetRawDataResource(
          new_tab_html_idr));

  std::string full_html = jstemplate_builder::GetI18nTemplateHtml(
      incognito_tab_html, &localized_strings);

  new_tab_incognito_html_ = base::RefCountedString::TakeString(&full_html);
}
