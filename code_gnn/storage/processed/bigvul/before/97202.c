void WebFrameLoaderClient::dispatchShow() {
  WebViewImpl* webview = webframe_->GetWebViewImpl();
  WebViewDelegate* d = webview->delegate();
  if (d)
    d->show(webview->initial_navigation_policy());
}
