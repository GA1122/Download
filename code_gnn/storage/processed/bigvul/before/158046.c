void LocalFrameClientImpl::DidChangeSelection(bool is_selection_empty) {
  if (web_frame_->Client())
    web_frame_->Client()->DidChangeSelection(is_selection_empty);
}
