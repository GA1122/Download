void RenderFrameHostImpl::SubresourceResponseStarted(const GURL& url,
                                                     const GURL& referrer,
                                                     const std::string& method,
                                                     ResourceType resource_type,
                                                     const std::string& ip,
                                                     uint32_t cert_status) {
  delegate_->SubresourceResponseStarted(url, referrer, method, resource_type,
                                        ip, cert_status);
}
