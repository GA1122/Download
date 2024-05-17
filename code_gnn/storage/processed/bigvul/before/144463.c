scoped_ptr<WebUIImpl> WebContentsImpl::CreateWebUIForRenderFrameHost(
    const GURL& url) {
  return scoped_ptr<WebUIImpl>(static_cast<WebUIImpl*>(CreateWebUI(
      url, std::string())));
}
