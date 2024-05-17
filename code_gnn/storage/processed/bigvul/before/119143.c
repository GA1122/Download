size_t CancelableSyncSocket::Receive(void* buffer, size_t length) {
  return CancelableFileOperation(
      &ReadFile, handle_, reinterpret_cast<char*>(buffer), length,
      &file_operation_, &shutdown_event_, this, INFINITE);
}
