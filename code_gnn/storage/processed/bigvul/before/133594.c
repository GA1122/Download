void WebContentsImpl::OnDidFailLoadWithError(
    int64 frame_id,
    const GURL& url,
    bool is_main_frame,
    int error_code,
    const base::string16& error_description) {
  GURL validated_url(url);
  RenderProcessHost* render_process_host =
      render_view_message_source_->GetProcess();
  render_process_host->FilterURL(false, &validated_url);
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidFailLoad(frame_id, validated_url, is_main_frame,
                                error_code, error_description,
                                render_view_message_source_));
}
