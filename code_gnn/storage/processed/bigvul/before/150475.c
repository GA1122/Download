bool NavigationControllerImpl::NavigateToPendingEntryInternal(
    ReloadType reload_type) {
  DCHECK(pending_entry_);
  FrameTreeNode* root = delegate_->GetFrameTree()->root();

  FrameLoadVector same_document_loads;
  FrameLoadVector different_document_loads;
  if (GetLastCommittedEntry()) {
    FindFramesToNavigate(root, &same_document_loads, &different_document_loads);
  }

  if (same_document_loads.empty() && different_document_loads.empty()) {
    different_document_loads.push_back(
        std::make_pair(root, pending_entry_->GetFrameEntry(root)));
  }

  bool success = false;

  for (const auto& item : same_document_loads) {
    FrameTreeNode* frame = item.first;
    success |= frame->navigator()->NavigateToPendingEntry(frame, *item.second,
                                                          reload_type, true);
  }
  for (const auto& item : different_document_loads) {
    FrameTreeNode* frame = item.first;
    success |= frame->navigator()->NavigateToPendingEntry(frame, *item.second,
                                                          reload_type, false);
  }
  return success;
}
