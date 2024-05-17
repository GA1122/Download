static uint32_t Read32LE(const uint8_t* p) {
  return p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}
