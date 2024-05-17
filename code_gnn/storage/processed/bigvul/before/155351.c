std::string ChromeContentBrowserClient::GetMetricSuffixForURL(const GURL& url) {
  if (page_load_metrics::IsGoogleSearchResultUrl(url))
    return "search";
  if (url.host() == "docs.google.com")
    return "docs";
  return std::string();
}
