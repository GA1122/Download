bool IsWebContentsBrowserPluginFocused(content::WebContents* web_contents) {
  WebContentsImpl* web_contents_impl =
      static_cast<WebContentsImpl*>(web_contents);
  BrowserPluginGuest* browser_plugin_guest =
      web_contents_impl->GetBrowserPluginGuest();
  return browser_plugin_guest ? browser_plugin_guest->focused() : false;
}
