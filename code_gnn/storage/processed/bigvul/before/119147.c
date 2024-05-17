size_t CancelableSyncSocket::Send(const void* buffer, size_t length) {
  static const DWORD kWaitTimeOutInMs = 500;
  return CancelableFileOperation(
      &WriteFile, handle_, reinterpret_cast<const char*>(buffer),
      length, &file_operation_, &shutdown_event_, this, kWaitTimeOutInMs);
}
