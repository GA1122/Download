base::string16 GetUrlWithLang(const GURL& url) {
  return base::ASCIIToUTF16(google_util::AppendGoogleLocaleParam(url).spec());
}
