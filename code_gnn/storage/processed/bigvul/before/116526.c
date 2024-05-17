bool ChromeWebUIControllerFactory::UseWebUIBindingsForURL(
    content::BrowserContext* browser_context, const GURL& url) const {
  return !NeedsExtensionWebUI(NULL,
                              Profile::FromBrowserContext(browser_context),
                              url) &&
       UseWebUIForURL(browser_context, url);
 }
