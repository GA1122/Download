void WebContentsImpl::WasUnOccluded() {
  if (!IsBeingCaptured())
    DoWasUnOccluded();

  should_normally_be_occluded_ = false;
}
