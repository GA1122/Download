static uint32_t Read32(const uint8_t* p) {
  return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}
