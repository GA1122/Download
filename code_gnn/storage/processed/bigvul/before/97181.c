void WebFrameLoaderClient::dispatchDidChangeLocationWithinPage() {
  WebViewImpl* webview = webframe_->GetWebViewImpl();
  if (webview->client())
    webview->client()->didStartLoading();

  WebDataSourceImpl* ds = webframe_->GetDataSourceImpl();
  DCHECK(ds) << "DataSource NULL when navigating to reference fragment";
  if (ds) {
    GURL url = ds->request().url();
    GURL chain_end = ds->endOfRedirectChain();
    ds->clearRedirectChain();

    bool was_client_redirect =
        (url == expected_client_redirect_dest_ &&
         chain_end == expected_client_redirect_src_) ||
        !webframe_->isProcessingUserGesture();

    if (was_client_redirect) {
      if (webframe_->client())
        webframe_->client()->didCompleteClientRedirect(webframe_, chain_end);
      ds->appendRedirect(chain_end);
      expected_client_redirect_src_ = GURL();
      expected_client_redirect_dest_ = GURL();
     }

    ds->appendRedirect(url);
  }

  bool is_new_navigation;
  webview->DidCommitLoad(&is_new_navigation);
  if (webframe_->client()) {
    webframe_->client()->didChangeLocationWithinPage(
        webframe_, is_new_navigation);
  }

  if (webview->client())
    webview->client()->didStopLoading();
}
