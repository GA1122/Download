void SafeBrowsingBlockingPage::PopulateMalwareStringDictionary(
    DictionaryValue* strings) {
  std::string diagnostic_link = base::StringPrintf(kSbDiagnosticHtml,
      l10n_util::GetStringUTF8(
        IDS_SAFE_BROWSING_MALWARE_DIAGNOSTIC_PAGE).c_str());

  strings->SetString("badURL", url().host());
  string16 description1, description3, description5;
  if (is_main_frame_load_blocked_) {
    description1 = l10n_util::GetStringFUTF16(
        IDS_SAFE_BROWSING_MALWARE_DESCRIPTION1, UTF8ToUTF16(url().host()));
  } else {
    description1 = l10n_util::GetStringFUTF16(
        IDS_SAFE_BROWSING_MALWARE_DESCRIPTION4,
        UTF8ToUTF16(tab()->GetURL().host()),
        UTF8ToUTF16(url().host()));
  }

  std::string proceed_link = base::StringPrintf(kPLinkHtml,
      l10n_util::GetStringUTF8(IDS_SAFE_BROWSING_MALWARE_PROCEED_LINK).c_str());
  description3 =
      l10n_util::GetStringFUTF16(IDS_SAFE_BROWSING_MALWARE_DESCRIPTION3,
                                 UTF8ToUTF16(proceed_link));

  PopulateStringDictionary(
      strings,
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_TITLE),
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_HEADLINE),
      description1,
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_DESCRIPTION2),
      description3);

  description5 =
      l10n_util::GetStringFUTF16(IDS_SAFE_BROWSING_MALWARE_DESCRIPTION5,
                                 UTF8ToUTF16(url().host()),
                                 UTF8ToUTF16(url().host()),
                                 UTF8ToUTF16(diagnostic_link));

  strings->SetString("description5", description5);

  strings->SetString("back_button",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_BACK_BUTTON));
  strings->SetString("proceed_link",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_PROCEED_LINK));
  strings->SetString("textdirection", base::i18n::IsRTL() ? "rtl" : "ltr");

  if (!CanShowMalwareDetailsOption()) {
    strings->SetBoolean(kDisplayCheckBox, false);
  } else {
    strings->SetBoolean(kDisplayCheckBox, true);

    std::string privacy_link = base::StringPrintf(
        kPrivacyLinkHtml,
        l10n_util::GetStringUTF8(
            IDS_SAFE_BROWSING_PRIVACY_POLICY_PAGE).c_str());

    strings->SetString("confirm_text",
                       l10n_util::GetStringFUTF16(
                           IDS_SAFE_BROWSING_MALWARE_REPORTING_AGREE,
                           UTF8ToUTF16(privacy_link)));

    Profile* profile = Profile::FromBrowserContext(tab()->browser_context());
    const PrefService::Preference* pref =
        profile->GetPrefs()->FindPreference(
            prefs::kSafeBrowsingReportingEnabled);

    bool value;
    if (pref && pref->GetValue()->GetAsBoolean(&value) && value) {
      strings->SetString(kBoxChecked, "yes");
    } else {
      strings->SetString(kBoxChecked, "");
    }
  }
}
