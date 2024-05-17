void NTPResourceCache::CreateNewTabIncognitoHTML() {
  base::DictionaryValue localized_strings;
  localized_strings.SetString("title",
      l10n_util::GetStringUTF16(IDS_NEW_TAB_TITLE));
  int new_tab_description_ids = IDS_NEW_TAB_OTR_DESCRIPTION;
  int new_tab_heading_ids = IDS_NEW_TAB_OTR_HEADING;
  int new_tab_link_ids = IDS_NEW_TAB_OTR_LEARN_MORE_LINK;
  int new_tab_warning_ids = IDS_NEW_TAB_OTR_MESSAGE_WARNING;
  int new_tab_html_idr = IDR_INCOGNITO_TAB_HTML;
  const char* new_tab_link = kLearnMoreIncognitoUrl;

  if (profile_->IsGuestSession()) {
    localized_strings.SetString("guestTabDescription",
        l10n_util::GetStringUTF16(new_tab_description_ids));
    localized_strings.SetString("guestTabHeading",
        l10n_util::GetStringUTF16(new_tab_heading_ids));
  } else {
    localized_strings.SetString("incognitoTabDescription",
        l10n_util::GetStringUTF16(new_tab_description_ids));
    localized_strings.SetString("incognitoTabHeading",
        l10n_util::GetStringUTF16(new_tab_heading_ids));
    localized_strings.SetString("incognitoTabWarning",
        l10n_util::GetStringUTF16(new_tab_warning_ids));
  }

  localized_strings.SetString("learnMore",
      l10n_util::GetStringUTF16(new_tab_link_ids));
  localized_strings.SetString("learnMoreLink",
      GetUrlWithLang(GURL(new_tab_link)));

  bool bookmark_bar_attached = profile_->GetPrefs()->GetBoolean(
      prefs::kShowBookmarkBar);
  localized_strings.SetBoolean("bookmarkbarattached", bookmark_bar_attached);

  webui::SetFontAndTextDirection(&localized_strings);

  static const base::StringPiece incognito_tab_html(
      ResourceBundle::GetSharedInstance().GetRawDataResource(
          new_tab_html_idr));

  std::string full_html = webui::GetI18nTemplateHtml(
      incognito_tab_html, &localized_strings);

  new_tab_incognito_html_ = base::RefCountedString::TakeString(&full_html);
}
