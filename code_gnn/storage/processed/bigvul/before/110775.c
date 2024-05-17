    AutocompleteEditModel::ClassifyPage(const GURL& gurl) const {
  if (!gurl.is_valid())
    return metrics::OmniboxEventProto_PageClassification_INVALID_SPEC;
  const std::string& url = gurl.spec();
  if (url == chrome::kChromeUINewTabURL)
    return metrics::OmniboxEventProto_PageClassification_NEW_TAB_PAGE;
  if (url == chrome::kAboutBlankURL)
    return metrics::OmniboxEventProto_PageClassification_BLANK;
  if (url == profile()->GetPrefs()->GetString(prefs::kHomePage))
    return metrics::OmniboxEventProto_PageClassification_HOMEPAGE;
  return metrics::OmniboxEventProto_PageClassification_OTHER;
}
