void WebContentsImpl::DidFailLoadWithError(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url,
    int error_code,
    const base::string16& error_description,
    bool was_ignored_by_handler) {
  FOR_EACH_OBSERVER(
      WebContentsObserver,
      observers_,
      DidFailLoad(render_frame_host, url, error_code, error_description,
        was_ignored_by_handler));
}
