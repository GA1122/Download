static uint32 Read32LE(const uint8* p) {
  return p[3] << 24 | p[2] << 16 | p[1] << 8 | p[0];
}
