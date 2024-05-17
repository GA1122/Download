void RenderViewHostImpl::OnDocumentOnLoadCompletedInMainFrame(
    int32 page_id) {
  delegate_->DocumentOnLoadCompletedInMainFrame(this, page_id);
}
