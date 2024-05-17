void WebLocalFrameImpl::WillDetachParent() {
  if (text_finder_ && text_finder_->ScopingInProgress()) {
    text_finder_->FlushCurrentScoping();

    text_finder_->CancelPendingScopingEffort();
  }
}
