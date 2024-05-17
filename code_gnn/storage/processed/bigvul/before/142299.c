bool ChromePasswordManagerClient::IsFillingFallbackEnabled(
    const GURL& url) const {
  return IsFillingEnabled(url) &&
         !Profile::FromBrowserContext(web_contents()->GetBrowserContext())
              ->IsGuestSession();
}
