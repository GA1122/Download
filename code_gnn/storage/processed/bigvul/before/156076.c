GURL GetGoogleSearchURL(const GURL& google_homepage_url) {
  GURL::Replacements replacements;
  replacements.SetPathStr("search");
  replacements.SetQueryStr("q=");
  return google_homepage_url.ReplaceComponents(replacements);
}
