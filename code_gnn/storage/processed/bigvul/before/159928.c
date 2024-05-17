bool MockCanAddURLToHistory(const GURL& url) {
  return url.is_valid() && !url.SchemeIs(kApplicationScheme);
}
