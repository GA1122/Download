static bool HandleWebUI(GURL* url, Profile* profile) {
  if (!ChromeWebUIFactory::GetInstance()->UseWebUIForURL(profile, *url))
    return false;

  if (url->SchemeIs(chrome::kChromeInternalScheme)) {
    *url = GURL(chrome::kChromeUINewTabURL);
  }

  return true;
}
