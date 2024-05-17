GURL GeneratePhishingReportUrl(const std::string& report_page,
                               const std::string& url_to_report) {
  icu::Locale locale = icu::Locale::getDefault();
  const char* lang = locale.getLanguage();
  if (!lang)
    lang = "en";   
  const std::string continue_esc =
      EscapeQueryParamValue(StringPrintf(kContinueUrlFormat, lang), true);
  const std::string current_esc = EscapeQueryParamValue(url_to_report, true);

#if defined(OS_WIN)
  BrowserDistribution* dist = BrowserDistribution::GetDistribution();
  std::string client_name(dist->GetSafeBrowsingName());
#else
  std::string client_name("googlechrome");
#endif

  GURL report_url(report_page +
      StringPrintf(kReportParams, client_name.c_str(), continue_esc.c_str(),
                   current_esc.c_str()));
  return google_util::AppendGoogleLocaleParam(report_url);
}
