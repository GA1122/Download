char GetCharFromIOVecs(size_t offset, iovec iov[], size_t count) {
  size_t start_offset = 0;
  for (size_t i = 0; i < count; i++) {
    if (iov[i].iov_len == 0) {
      continue;
    }
    size_t end_offset = start_offset + iov[i].iov_len - 1;
    if (offset >= start_offset && offset <= end_offset) {
      const char* buf = reinterpret_cast<const char*>(iov[i].iov_base);
      return buf[offset - start_offset];
    }
    start_offset += iov[i].iov_len;
  }
  LOG(ERROR) << "Could not locate char at offset " << offset << " in " << count
             << " iovecs";
  for (size_t i = 0; i < count; ++i) {
    LOG(ERROR) << "  iov[" << i << "].iov_len = " << iov[i].iov_len;
  }
  return '\0';
}
