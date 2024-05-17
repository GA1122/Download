bool IsDataOrAbout(const GURL& url) {
  GURL about_srcdoc(content::kAboutSrcDocURL);
  return url == about_srcdoc || url.IsAboutBlank() ||
         url.scheme() == url::kDataScheme;
}
