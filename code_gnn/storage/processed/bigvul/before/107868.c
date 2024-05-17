WebViewPlugin::WebViewPlugin(WebViewPlugin::Delegate* delegate)
    : delegate_(delegate),
      container_(NULL),
      finished_loading_(false) {
  web_view_ = WebView::create(this, NULL, NULL);
  web_view_->initializeMainFrame(this);
}
