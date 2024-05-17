void SafeBrowsingBlockingPage::PopulateMultipleThreatStringDictionary(
    DictionaryValue* strings) {
  bool malware = false;
  bool phishing = false;

  string16 malware_label =
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_LABEL);
  string16 malware_link =
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_DIAGNOSTIC_PAGE);
  string16 phishing_label =
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_LABEL);
  string16 phishing_link =
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_REPORT_ERROR);

  ListValue* error_strings = new ListValue;
  for (UnsafeResourceList::const_iterator iter = unsafe_resources_.begin();
       iter != unsafe_resources_.end(); ++iter) {
    const SafeBrowsingService::UnsafeResource& resource = *iter;
    SafeBrowsingService::UrlCheckResult threat_type = resource.threat_type;
    DictionaryValue* current_error_strings = new DictionaryValue;
    if (threat_type == SafeBrowsingService::URL_MALWARE) {
      malware = true;
      current_error_strings->SetString("type", "malware");
      current_error_strings->SetString("typeLabel", malware_label);
      current_error_strings->SetString("errorLink", malware_link);
    } else {
      DCHECK(threat_type == SafeBrowsingService::URL_PHISHING ||
             threat_type == SafeBrowsingService::CLIENT_SIDE_PHISHING_URL);
      phishing = true;
      current_error_strings->SetString("type", "phishing");
      current_error_strings->SetString("typeLabel", phishing_label);
      current_error_strings->SetString("errorLink", phishing_link);
    }
    current_error_strings->SetString("url", resource.url.spec());
    error_strings->Append(current_error_strings);
  }
  strings->Set("errors", error_strings);
  DCHECK(phishing || malware);

  if (malware && phishing) {
    PopulateStringDictionary(
        strings,
        l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MULTI_THREAT_TITLE),
        l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_HEADLINE),
        l10n_util::GetStringFUTF16(IDS_SAFE_BROWSING_MULTI_THREAT_DESCRIPTION1,
                                   UTF8ToUTF16(tab()->GetURL().host())),
        l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MULTI_THREAT_DESCRIPTION2),
        string16());
  } else if (malware) {
    PopulateStringDictionary(
        strings,
        l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_TITLE),
        l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_HEADLINE),
        l10n_util::GetStringFUTF16(IDS_SAFE_BROWSING_MULTI_MALWARE_DESCRIPTION1,
                                   UTF8ToUTF16(tab()->GetURL().host())),
        l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MULTI_MALWARE_DESCRIPTION2),
        l10n_util::GetStringUTF16(
            IDS_SAFE_BROWSING_MULTI_MALWARE_DESCRIPTION3));
  } else {
    PopulateStringDictionary(
        strings,
        l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_TITLE),
        l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_HEADLINE),
        l10n_util::GetStringFUTF16(
            IDS_SAFE_BROWSING_MULTI_PHISHING_DESCRIPTION1,
            UTF8ToUTF16(tab()->GetURL().host())),
        string16(),
        string16());
  }

  strings->SetString("confirm_text",
                     l10n_util::GetStringUTF16(
                         IDS_SAFE_BROWSING_MULTI_MALWARE_DESCRIPTION_AGREE));
  strings->SetString("continue_button",
                     l10n_util::GetStringUTF16(
                         IDS_SAFE_BROWSING_MULTI_MALWARE_PROCEED_BUTTON));
  strings->SetString("back_button",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_MALWARE_BACK_BUTTON));
  strings->SetString("textdirection", base::i18n::IsRTL() ? "rtl" : "ltr");
}
