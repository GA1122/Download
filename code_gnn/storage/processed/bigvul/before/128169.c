PluginDataRemover* PluginDataRemover::Create(BrowserContext* browser_context) {
  return new PluginDataRemoverImpl(browser_context);
}
