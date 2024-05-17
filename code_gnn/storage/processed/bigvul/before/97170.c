void WebFrameLoaderClient::didCreateScriptContextForFrame() {
  WebViewImpl* webview = webframe_->GetWebViewImpl();
  WebViewDelegate* d = webview->delegate();
  if (d)
    d->DidCreateScriptContextForFrame(webframe_);
}
