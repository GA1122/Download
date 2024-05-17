void UDPSocketWin::Core::ReadDelegate::OnObjectSignaled(HANDLE object) {
  DCHECK_EQ(object, core_->read_overlapped_.hEvent);
  if (core_->socket_)
    core_->socket_->DidCompleteRead();

  core_->Release();
}
