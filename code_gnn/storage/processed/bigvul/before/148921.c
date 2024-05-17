void RenderFrameHostManager::UpdatePendingWebUIOnCurrentFrameHost(
    const GURL& dest_url,
    int entry_bindings) {
  bool pending_webui_changed =
      render_frame_host_->UpdatePendingWebUI(dest_url, entry_bindings);
  DCHECK_EQ(GetNavigatingWebUI(), render_frame_host_->pending_web_ui());

  if (render_frame_host_->pending_web_ui() && pending_webui_changed &&
      render_frame_host_->IsRenderFrameLive()) {
    if (render_frame_host_->pending_web_ui() == render_frame_host_->web_ui()) {
      render_frame_host_->pending_web_ui()->RenderFrameReused(
          render_frame_host_.get());
    } else {
      render_frame_host_->pending_web_ui()->RenderFrameCreated(
          render_frame_host_.get());
    }
  }
}
