void RenderFrameHostImpl::OnDidStartLoading(bool to_different_document) {
  if (IsBrowserSideNavigationEnabled() && to_different_document) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RFH_UNEXPECTED_LOAD_START);
    return;
  }
  bool was_previously_loading = frame_tree_node_->frame_tree()->IsLoading();
  is_loading_ = true;

  if (is_active()) {
    frame_tree_node_->DidStartLoading(to_different_document,
                                      was_previously_loading);
  }
}
