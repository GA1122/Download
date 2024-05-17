size_t SyncSocket::Send(const void* buffer, size_t length) {
  ThreadRestrictions::AssertIOAllowed();
  DCHECK_GT(length, 0u);
  DCHECK_LE(length, kMaxMessageLength);
  DCHECK_NE(handle_, kInvalidHandle);
  size_t count = 0;
  while (count < length) {
    DWORD len;
    DWORD chunk = GetNextChunkSize(count, length);
    if (WriteFile(handle_, static_cast<const char*>(buffer) + count,
                  chunk, &len, NULL) == FALSE) {
      return count;
    }
    count += len;
  }
  return count;
}
