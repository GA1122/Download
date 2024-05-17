void WebFrameLoaderClient::postProgressStartedNotification() {
  WebViewImpl* webview = webframe_->GetWebViewImpl();
  if (webview && webview->client())
    webview->client()->didStartLoading();
}
