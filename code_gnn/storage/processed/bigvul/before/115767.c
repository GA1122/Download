void SafeBrowsingBlockingPage::PopulatePhishingStringDictionary(
    DictionaryValue* strings) {
  std::string proceed_link = base::StringPrintf(
      kPLinkHtml,
      l10n_util::GetStringUTF8(
          IDS_SAFE_BROWSING_PHISHING_PROCEED_LINK).c_str());
  string16 description3 = l10n_util::GetStringFUTF16(
      IDS_SAFE_BROWSING_PHISHING_DESCRIPTION3,
      UTF8ToUTF16(proceed_link));

  PopulateStringDictionary(
      strings,
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_TITLE),
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_HEADLINE),
      l10n_util::GetStringFUTF16(IDS_SAFE_BROWSING_PHISHING_DESCRIPTION1,
                                 UTF8ToUTF16(url().host())),
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_DESCRIPTION2),
      description3);

  strings->SetString("back_button",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_BACK_BUTTON));
  strings->SetString("report_error",
      l10n_util::GetStringUTF16(IDS_SAFE_BROWSING_PHISHING_REPORT_ERROR));
  strings->SetString("textdirection", base::i18n::IsRTL() ? "rtl" : "ltr");
}
