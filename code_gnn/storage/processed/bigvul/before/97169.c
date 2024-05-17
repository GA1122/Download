void WebFrameLoaderClient::didCreateIsolatedScriptContext() {
  WebViewImpl* webview = webframe_->GetWebViewImpl();
  WebViewDelegate* d = webview->delegate();
  if (d)
    d->DidCreateIsolatedScriptContext(webframe_);
}
