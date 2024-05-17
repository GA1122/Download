GURL AppendGoogleLocaleParam(const GURL& url,
                             const std::string& application_locale) {
  return net::AppendQueryParameter(url, "hl",
                                   GetGoogleLocale(application_locale));
}
