static uint32_t Read24(const uint8_t* p) {
  return p[0] << 16 | p[1] << 8 | p[2];
}
