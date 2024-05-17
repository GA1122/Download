bool Extension::CanSpecifyHostPermission(const URLPattern& pattern) const {
  if (!pattern.match_all_urls() &&
      pattern.MatchesScheme(chrome::kChromeUIScheme)) {
    return (pattern.host() == chrome::kChromeUIFaviconHost ||
            CanExecuteScriptEverywhere());
  }

  return true;
}
