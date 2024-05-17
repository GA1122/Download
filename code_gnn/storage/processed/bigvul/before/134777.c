void EventConverterEvdevImpl::OnStopped() {
  ReleaseKeys();
  ReleaseMouseButtons();
}
