WebUIController* NewWebUI<ExtensionWebUI>(content::WebUI* web_ui,
                                          const GURL& url) {
  return new ExtensionWebUI(web_ui, url);
}
