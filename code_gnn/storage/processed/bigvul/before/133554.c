void WebContentsImpl::DidStartProvisionalLoad(
    RenderFrameHostImpl* render_frame_host,
    int64 frame_id,
    int64 parent_frame_id,
    bool is_main_frame,
    const GURL& validated_url,
    bool is_error_page,
    bool is_iframe_srcdoc) {
  if (is_main_frame)
    DidChangeLoadProgress(0);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidStartProvisionalLoadForFrame(frame_id, parent_frame_id,
                    is_main_frame, validated_url, is_error_page,
                    is_iframe_srcdoc, render_frame_host->render_view_host()));

  if (is_main_frame) {
    FOR_EACH_OBSERVER(
        WebContentsObserver,
        observers_,
        ProvisionalChangeToMainFrameUrl(validated_url,
                                        render_frame_host));
  }
}
