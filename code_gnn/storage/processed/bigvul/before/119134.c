bool SyncSocket::Close() {
  if (handle_ == kInvalidHandle)
    return true;

  const BOOL result = CloseHandle(handle_);
  handle_ = kInvalidHandle;
  return result == TRUE;
}
