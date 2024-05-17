bool IsURLAllowedInIncognito(const GURL& url,
                             content::BrowserContext* browser_context) {
  if (url.scheme() == content::kViewSourceScheme) {
    std::string stripped_spec = url.spec();
    DCHECK_GT(stripped_spec.size(), strlen(content::kViewSourceScheme));
    stripped_spec.erase(0, strlen(content::kViewSourceScheme) + 1);
    GURL stripped_url(stripped_spec);
    return stripped_url.is_valid() &&
        IsURLAllowedInIncognito(stripped_url, browser_context);
  }
  if (url.scheme() == content::kChromeUIScheme &&
      (url.host_piece() == chrome::kChromeUIAppLauncherPageHost ||
       url.host_piece() == chrome::kChromeUISettingsHost ||
       url.host_piece() == chrome::kChromeUIHelpHost ||
       url.host_piece() == chrome::kChromeUIHistoryHost ||
       url.host_piece() == chrome::kChromeUIExtensionsHost ||
       url.host_piece() == chrome::kChromeUIBookmarksHost ||
       url.host_piece() == chrome::kChromeUIChromeSigninHost ||
       url.host_piece() == chrome::kChromeUIUberHost ||
       url.host_piece() == chrome::kChromeUIThumbnailHost ||
       url.host_piece() == chrome::kChromeUIThumbnailHost2 ||
       url.host_piece() == chrome::kChromeUIThumbnailListHost ||
       url.host_piece() == chrome::kChromeUISuggestionsHost ||
       url.host_piece() == chrome::kChromeUIDevicesHost)) {
    return false;
  }

  if (url.scheme() == chrome::kChromeSearchScheme &&
      (url.host_piece() == chrome::kChromeUIThumbnailHost ||
       url.host_piece() == chrome::kChromeUIThumbnailHost2 ||
       url.host_piece() == chrome::kChromeUIThumbnailListHost ||
       url.host_piece() == chrome::kChromeUISuggestionsHost)) {
    return false;
  }

  GURL rewritten_url = url;
  bool reverse_on_redirect = false;
  content::BrowserURLHandler::GetInstance()->RewriteURLIfNecessary(
      &rewritten_url, browser_context, &reverse_on_redirect);

  return !(rewritten_url.scheme_piece() == content::kChromeUIScheme &&
           rewritten_url.host_piece() == chrome::kChromeUIUberHost);
}
