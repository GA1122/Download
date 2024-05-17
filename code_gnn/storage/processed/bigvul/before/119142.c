size_t SyncSocket::Receive(void* buffer, size_t length) {
  ThreadRestrictions::AssertIOAllowed();
  DCHECK_GT(length, 0u);
  DCHECK_LE(length, kMaxMessageLength);
  DCHECK_NE(handle_, kInvalidHandle);
  size_t count = 0;
  while (count < length) {
    DWORD len;
    DWORD chunk = GetNextChunkSize(count, length);
    if (ReadFile(handle_, static_cast<char*>(buffer) + count,
                 chunk, &len, NULL) == FALSE) {
      return count;
    }
    count += len;
  }
  return count;
}
