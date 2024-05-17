void Dispatcher::UpdateOriginPermissions(const GURL& extension_url,
                                         const URLPatternSet& old_patterns,
                                         const URLPatternSet& new_patterns) {
  static const char* kSchemes[] = {
    url::kHttpScheme,
    url::kHttpsScheme,
    url::kFileScheme,
    content::kChromeUIScheme,
    url::kFtpScheme,
#if defined(OS_CHROMEOS)
    content::kExternalFileScheme,
#endif
    extensions::kExtensionScheme,
  };
  for (size_t i = 0; i < arraysize(kSchemes); ++i) {
    const char* scheme = kSchemes[i];
    for (URLPatternSet::const_iterator pattern = old_patterns.begin();
         pattern != old_patterns.end(); ++pattern) {
      if (pattern->MatchesScheme(scheme)) {
        WebSecurityPolicy::removeOriginAccessWhitelistEntry(
            extension_url,
            WebString::fromUTF8(scheme),
            WebString::fromUTF8(pattern->host()),
            pattern->match_subdomains());
      }
    }
    for (URLPatternSet::const_iterator pattern = new_patterns.begin();
         pattern != new_patterns.end(); ++pattern) {
      if (pattern->MatchesScheme(scheme)) {
        WebSecurityPolicy::addOriginAccessWhitelistEntry(
            extension_url,
            WebString::fromUTF8(scheme),
            WebString::fromUTF8(pattern->host()),
            pattern->match_subdomains());
      }
    }
  }
}
