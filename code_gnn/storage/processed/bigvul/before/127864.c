static bool CheckHls(const uint8* buffer, int buffer_size) {

  if (StartsWith(buffer, buffer_size, kHlsSignature)) {
    int offset = strlen(kHlsSignature);
    while (offset < buffer_size) {
      if (buffer[offset] == '#') {
        if (StartsWith(buffer + offset, buffer_size - offset, kHls1) ||
            StartsWith(buffer + offset, buffer_size - offset, kHls2) ||
            StartsWith(buffer + offset, buffer_size - offset, kHls3)) {
          return true;
        }
      }
      ++offset;
    }
  }
  return false;
}
