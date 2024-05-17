static uint64 GetVint(BitReader* reader) {
  if (reader->bits_available() >= 8) {
    int num_bits_to_read = 0;
    for (int i = 0; i < 8; ++i) {
      num_bits_to_read += 7;
      if (ReadBits(reader, 1) == 1) {
        if (reader->bits_available() < num_bits_to_read)
          break;
        return ReadBits(reader, num_bits_to_read);
      }
    }
  }
  return (reader->bits_available() / 8) + 2;
}
