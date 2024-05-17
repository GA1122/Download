void WebFrameLoaderClient::HandleBackForwardNavigation(const GURL& url) {
  DCHECK(url.SchemeIs(webkit_glue::kBackForwardNavigationScheme));

  std::string offset_str = url.ExtractFileName();
  int offset;
  if (!StringToInt(offset_str, &offset))
    return;

  WebViewImpl* webview = webframe_->GetWebViewImpl();
  if (webview->client())
    webview->client()->navigateBackForwardSoon(offset);
}
