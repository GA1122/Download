WebUIController* NewWebUI<AboutUI>(content::WebUI* web_ui, const GURL& url) {
  return new AboutUI(web_ui, url.host());
}
