WebContents* WebContentsImpl::OpenURL(const OpenURLParams& params) {
  if (!delegate_)
    return nullptr;

  WebContents* new_contents = delegate_->OpenURLFromTab(this, params);

  RenderFrameHost* source_render_frame_host = RenderFrameHost::FromID(
      params.source_render_process_id, params.source_render_frame_id);

  if (source_render_frame_host && params.source_site_instance) {
    CHECK_EQ(source_render_frame_host->GetSiteInstance(),
             params.source_site_instance.get());
  }

  if (new_contents && source_render_frame_host && new_contents != this) {
    for (auto& observer : observers_) {
      observer.DidOpenRequestedURL(new_contents, source_render_frame_host,
                                   params.url, params.referrer,
                                   params.disposition, params.transition,
                                   params.started_from_context_menu, false);
    }
  }

  return new_contents;
}
