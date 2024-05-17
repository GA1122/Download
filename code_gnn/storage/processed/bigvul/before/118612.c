void NTPResourceCache::CreateNewTabGuestHTML() {
  base::DictionaryValue localized_strings;
  localized_strings.SetString("title",
      l10n_util::GetStringUTF16(IDS_NEW_TAB_TITLE));
  const char* guest_tab_link = kLearnMoreGuestSessionUrl;
  int guest_tab_ids = IDR_GUEST_TAB_HTML;
  int guest_tab_description_ids = IDS_NEW_TAB_GUEST_SESSION_DESCRIPTION;
  int guest_tab_heading_ids = IDS_NEW_TAB_GUEST_SESSION_HEADING;
  int guest_tab_link_ids = IDS_NEW_TAB_GUEST_SESSION_LEARN_MORE_LINK;

#if defined(OS_CHROMEOS)
  guest_tab_ids = IDR_GUEST_SESSION_TAB_HTML;
  guest_tab_link = kLearnMoreGuestSessionUrl;

  policy::BrowserPolicyConnectorChromeOS* connector =
      g_browser_process->platform_part()->browser_policy_connector_chromeos();
  std::string enterprise_domain = connector->GetEnterpriseDomain();

  if (!enterprise_domain.empty()) {
    localized_strings.SetString("enterpriseInfoVisible", "true");
    base::string16 enterprise_info = l10n_util::GetStringFUTF16(
        IDS_DEVICE_OWNED_BY_NOTICE,
        base::UTF8ToUTF16(enterprise_domain));
    localized_strings.SetString("enterpriseInfoMessage", enterprise_info);
    localized_strings.SetString("enterpriseLearnMore",
        l10n_util::GetStringUTF16(IDS_LEARN_MORE));
    localized_strings.SetString("enterpriseInfoHintLink",
        GetUrlWithLang(GURL(chrome::kLearnMoreEnterpriseURL)));
  } else {
    localized_strings.SetString("enterpriseInfoVisible", "false");
  }
#endif

  localized_strings.SetString("guestTabDescription",
      l10n_util::GetStringUTF16(guest_tab_description_ids));
  localized_strings.SetString("guestTabHeading",
      l10n_util::GetStringUTF16(guest_tab_heading_ids));
  localized_strings.SetString("learnMore",
      l10n_util::GetStringUTF16(guest_tab_link_ids));
  localized_strings.SetString("learnMoreLink",
      GetUrlWithLang(GURL(guest_tab_link)));

  webui::SetFontAndTextDirection(&localized_strings);

  static const base::StringPiece guest_tab_html(
      ResourceBundle::GetSharedInstance().GetRawDataResource(guest_tab_ids));

  std::string full_html = webui::GetI18nTemplateHtml(
      guest_tab_html, &localized_strings);

  new_tab_guest_html_ = base::RefCountedString::TakeString(&full_html);
}
