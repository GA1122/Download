WebUIController* NewWebUI(content::WebUI* web_ui, const GURL& url) {
  return new T(web_ui);
}
