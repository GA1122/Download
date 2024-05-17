void Document::DispatchDidReceiveTitle() {
  frame_->Client()->DispatchDidReceiveTitle(title_);
}
