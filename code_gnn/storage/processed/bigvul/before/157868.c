void WebContentsImpl::SetWasDiscarded(bool was_discarded) {
  GetFrameTree()->root()->set_was_discarded();
}
