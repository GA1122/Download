void WebContentsImpl::DidRedirectProvisionalLoad(
    RenderFrameHostImpl* render_frame_host,
    const GURL& validated_target_url) {
  FOR_EACH_OBSERVER(
      WebContentsObserver,
      observers_,
      ProvisionalChangeToMainFrameUrl(validated_target_url,
                                      render_frame_host));
}
