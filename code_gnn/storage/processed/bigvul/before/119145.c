size_t CancelableSyncSocket::ReceiveWithTimeout(void* buffer,
                                                size_t length,
                                                TimeDelta timeout) {
  return CancelableFileOperation(
      &ReadFile, handle_, reinterpret_cast<char*>(buffer), length,
      &file_operation_, &shutdown_event_, this,
      static_cast<DWORD>(timeout.InMilliseconds()));
}
