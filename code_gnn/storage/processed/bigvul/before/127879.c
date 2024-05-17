static uint32 Read32(const uint8* p) {
  return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}
