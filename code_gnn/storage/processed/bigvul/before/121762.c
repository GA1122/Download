void UDPSocketLibevent::ReadWatcher::OnFileCanReadWithoutBlocking(int) {
  if (!socket_->read_callback_.is_null())
    socket_->DidCompleteRead();
}
