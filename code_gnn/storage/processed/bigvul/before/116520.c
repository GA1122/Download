 bool ChromeWebUIControllerFactory::IsURLAcceptableForWebUI(
     content::BrowserContext* browser_context,
     const GURL& url) const {
  return UseWebUIForURL(browser_context, url) ||
      url.SchemeIs(chrome::kJavaScriptScheme) ||
      url.spec() == chrome::kAboutBlankURL ||
      url == GURL(chrome::kChromeUICrashURL) ||
      url == GURL(chrome::kChromeUIKillURL) ||
      url == GURL(chrome::kChromeUIHangURL) ||
      url == GURL(chrome::kChromeUIShorthangURL);
}
