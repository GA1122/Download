bool IsLocalFile(const GURL& url) {
  return url.SchemeIsFile() || url.SchemeIsFileSystem() ||
         url.SchemeIs(url::kContentScheme) ||
         url.SchemeIs(url::kContentIDScheme) ||
         url.SchemeIs("chrome-extension");
}
