WebViewPlugin* WebViewPlugin::Create(WebViewPlugin::Delegate* delegate,
                                     const WebPreferences& preferences,
                                     const std::string& html_data,
                                     const GURL& url) {
  WebViewPlugin* plugin = new WebViewPlugin(delegate);
  WebView* web_view = plugin->web_view();
  preferences.Apply(web_view);
  web_view->mainFrame()->loadHTMLString(html_data, url);
  return plugin;
}
