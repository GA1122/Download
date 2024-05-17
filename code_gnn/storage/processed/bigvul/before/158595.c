void WebLocalFrameImpl::WillDetachParent() {
  if (GetTextFinder() && GetTextFinder()->ScopingInProgress()) {
    GetTextFinder()->FlushCurrentScoping();

    GetTextFinder()->CancelPendingScopingEffort();
  }
}
