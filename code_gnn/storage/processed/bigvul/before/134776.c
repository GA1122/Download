void EventConverterEvdevImpl::OnLostSync() {
  LOG(WARNING) << "kernel dropped input events";

  ReleaseKeys();
  ReleaseMouseButtons();
}
