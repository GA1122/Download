static bool CheckGsm(const uint8_t* buffer, int buffer_size) {
  RCHECK(buffer_size >= 1024);   

  int offset = 0;
  while (offset < buffer_size) {
    RCHECK((buffer[offset] & 0xf0) == 0xd0);
    offset += 33;
  }
  return true;
}
