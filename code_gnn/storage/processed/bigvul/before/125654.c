void RenderViewHostImpl::OnDidStartProvisionalLoadForFrame(
    int64 frame_id,
    int64 parent_frame_id,
    bool is_main_frame,
    const GURL& url) {
  delegate_->DidStartProvisionalLoadForFrame(
      this, frame_id, parent_frame_id, is_main_frame, url);
}
