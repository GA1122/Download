void WebFrameLoaderClient::postProgressFinishedNotification() {
  WebViewImpl* webview = webframe_->GetWebViewImpl();
  if (webview && webview->client())
    webview->client()->didStopLoading();
}
