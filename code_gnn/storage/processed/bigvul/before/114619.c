  virtual bool Visit(content::RenderView* render_view) {
    WebView* webview = render_view->GetWebView();
    WebDocument document = webview->mainFrame()->document();

    if (document.isPluginDocument())
      return true;

    if (net::GetHostOrSpecFromURL(GURL(document.url())) == host_)
      webview->setZoomLevel(false, zoom_level_);
    return true;
  }
