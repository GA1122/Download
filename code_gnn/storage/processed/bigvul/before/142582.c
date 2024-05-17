WebstoreAPI* WebstoreAPI::Get(content::BrowserContext* browser_context) {
  return BrowserContextKeyedAPIFactory<WebstoreAPI>::Get(browser_context);
}
