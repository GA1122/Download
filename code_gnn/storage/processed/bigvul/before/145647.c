bool HasWebRequestScheme(const GURL& url) {
  return (url.SchemeIs(url::kAboutScheme) || url.SchemeIs(url::kFileScheme) ||
          url.SchemeIs(url::kFileSystemScheme) ||
          url.SchemeIs(url::kFtpScheme) || url.SchemeIs(url::kHttpScheme) ||
          url.SchemeIs(url::kHttpsScheme) ||
          url.SchemeIs(extensions::kExtensionScheme));
}
