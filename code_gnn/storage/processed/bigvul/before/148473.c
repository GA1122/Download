void WebContentsImpl::OnDidRunInsecureContent(RenderFrameHostImpl* source,
                                              const GURL& security_origin,
                                              const GURL& target_url) {
  DidRunInsecureContent(security_origin, target_url);
}
