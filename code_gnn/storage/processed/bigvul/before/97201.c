void WebFrameLoaderClient::dispatchDidStartProvisionalLoad() {
  WebDataSourceImpl* ds = webframe_->GetProvisionalDataSourceImpl();
  if (!ds) {
    NOTREACHED() << "Attempting to provisional load but there isn't one";
    return;
  }
  GURL url = ds->request().url();

  DCHECK(!ds->hasRedirectChain());

  bool completing_client_redirect = false;
  if (expected_client_redirect_src_.is_valid()) {
    DCHECK(expected_client_redirect_dest_.SchemeIs("javascript") ||
           expected_client_redirect_dest_ == url);
    ds->appendRedirect(expected_client_redirect_src_);
    completing_client_redirect = true;
  }
  ds->appendRedirect(url);

  if (webframe_->client()) {
    webframe_->client()->didStartProvisionalLoad(webframe_);
    if (completing_client_redirect)
      webframe_->client()->didCompleteClientRedirect(
          webframe_, expected_client_redirect_src_);
  }
}
