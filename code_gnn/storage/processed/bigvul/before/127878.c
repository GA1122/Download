static uint32 Read24(const uint8* p) {
  return p[0] << 16 | p[1] << 8 | p[2];
}
