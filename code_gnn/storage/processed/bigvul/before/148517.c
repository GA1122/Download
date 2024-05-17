void WebContentsImpl::PassiveInsecureContentFound(const GURL& resource_url) {
  GetDelegate()->PassiveInsecureContentFound(resource_url);
}
