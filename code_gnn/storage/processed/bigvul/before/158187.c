bool IsURLHandledByDefaultLoader(const GURL& url) {
  return IsURLHandledByNetworkService(url) || url.SchemeIs(url::kDataScheme);
}
