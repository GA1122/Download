void WebContentsImpl::WasUnOccluded() {
  if (capturer_count_ == 0)
    DoWasUnOccluded();

  should_normally_be_occluded_ = false;
}
