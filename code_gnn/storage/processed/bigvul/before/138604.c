bool NavigatorImpl::NavigateToPendingEntry(
    FrameTreeNode* frame_tree_node,
    const FrameNavigationEntry& frame_entry,
    ReloadType reload_type,
    bool is_same_document_history_load) {
  return NavigateToEntry(frame_tree_node, frame_entry,
                         *controller_->GetPendingEntry(), reload_type,
                         is_same_document_history_load, false, true, nullptr);
}
