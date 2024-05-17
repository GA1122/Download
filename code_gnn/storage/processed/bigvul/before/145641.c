bool ShouldTreatURLSchemeAsLegacy(const GURL& url) {
  return url.SchemeIs(url::kFtpScheme) || url.SchemeIs(url::kGopherScheme);
}
