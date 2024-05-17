void CommandBufferProxyImpl::SetPresentationCallback(
    const PresentationCallback& callback) {
  CheckLock();
  presentation_callback_ = callback;
}
