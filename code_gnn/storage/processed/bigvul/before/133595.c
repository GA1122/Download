void WebContentsImpl::OnDidFinishLoad(
    int64 frame_id,
    const GURL& url,
    bool is_main_frame) {
  GURL validated_url(url);

  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kSitePerProcess) &&
      render_view_message_source_ != GetRenderViewHost())
    is_main_frame = false;

  RenderProcessHost* render_process_host =
      render_view_message_source_->GetProcess();
  render_process_host->FilterURL(false, &validated_url);
  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidFinishLoad(frame_id, validated_url, is_main_frame,
                                  render_view_message_source_));
}
