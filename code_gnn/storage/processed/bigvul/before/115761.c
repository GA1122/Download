std::string SafeBrowsingBlockingPage::GetHTMLContents() {
  DictionaryValue strings;
  ResourceBundle& rb = ResourceBundle::GetSharedInstance();
  std::string html;

  if (unsafe_resources_.empty()) {
    NOTREACHED();
    return std::string();
  }

  if (unsafe_resources_.size() > 1) {
    PopulateMultipleThreatStringDictionary(&strings);
    html = rb.GetRawDataResource(
        IDR_SAFE_BROWSING_MULTIPLE_THREAT_BLOCK).as_string();
  } else {
    SafeBrowsingService::UrlCheckResult threat_type =
        unsafe_resources_[0].threat_type;
    if (threat_type == SafeBrowsingService::URL_MALWARE) {
      PopulateMalwareStringDictionary(&strings);
      html = rb.GetRawDataResource(
          IDR_SAFE_BROWSING_MALWARE_BLOCK).as_string();
    } else {   
      DCHECK(threat_type == SafeBrowsingService::URL_PHISHING ||
             threat_type == SafeBrowsingService::CLIENT_SIDE_PHISHING_URL);
      PopulatePhishingStringDictionary(&strings);
      html = rb.GetRawDataResource(
          IDR_SAFE_BROWSING_PHISHING_BLOCK).as_string();
    }
  }

  return jstemplate_builder::GetTemplatesHtml(html, &strings, "template_root");
}
