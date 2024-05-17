void LocalFrameClientImpl::DidFinishSameDocumentNavigation(
    HistoryItem* item,
    WebHistoryCommitType commit_type,
    bool content_initiated) {
  bool should_create_history_entry = commit_type == kWebStandardCommit;
  web_frame_->ViewImpl()->DidCommitLoad(should_create_history_entry, true);
  if (web_frame_->Client()) {
    web_frame_->Client()->DidFinishSameDocumentNavigation(
        WebHistoryItem(item), commit_type, content_initiated);
  }
  virtual_time_pauser_.UnpauseVirtualTime();
}
