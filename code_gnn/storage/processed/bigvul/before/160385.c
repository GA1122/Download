static int numberOfLeadingZeroes(uint8_t byte) {
  if (!byte)
    return 8;
  int result = 0;
  if (byte <= 0x0F) {
    result += 4;
    byte = byte << 4;
  }
  if (byte <= 0x3F) {
    result += 2;
    byte = byte << 2;
  }
  if (byte <= 0x7F)
    result++;
  return result;
}
