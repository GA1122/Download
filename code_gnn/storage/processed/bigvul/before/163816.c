void ChromeContentBrowserClientExtensionsPart::RecordShouldAllowOpenURLFailure(
    ShouldAllowOpenURLFailureReason reason,
    const GURL& site_url) {
  UMA_HISTOGRAM_ENUMERATION("Extensions.ShouldAllowOpenURL.Failure", reason,
                            FAILURE_LAST);

  static const char* const kSchemeNames[] = {
      "unknown",
      "",
      url::kHttpScheme,
      url::kHttpsScheme,
      url::kFileScheme,
      url::kFtpScheme,
      url::kDataScheme,
      url::kJavaScriptScheme,
      url::kAboutScheme,
      content::kChromeUIScheme,
      content::kChromeDevToolsScheme,
      content::kGuestScheme,
      content::kViewSourceScheme,
      chrome::kChromeSearchScheme,
      chrome::kChromeNativeScheme,
      dom_distiller::kDomDistillerScheme,
      extensions::kExtensionScheme,
      url::kContentScheme,
      url::kBlobScheme,
      url::kFileSystemScheme,
      "last",
  };

  static_assert(arraysize(kSchemeNames) == SCHEME_LAST + 1,
                "kSchemeNames should have SCHEME_LAST + 1 elements");

  ShouldAllowOpenURLFailureScheme scheme = SCHEME_UNKNOWN;
  for (int i = 1; i < SCHEME_LAST; i++) {
    if (site_url.SchemeIs(kSchemeNames[i])) {
      scheme = static_cast<ShouldAllowOpenURLFailureScheme>(i);
      break;
    }
  }

  UMA_HISTOGRAM_ENUMERATION("Extensions.ShouldAllowOpenURL.Failure.Scheme",
                            scheme, SCHEME_LAST);
}
