static bool CheckEac3(const uint8* buffer, int buffer_size) {

  RCHECK(buffer_size > 6);

  int offset = 0;
  while (offset + 6 < buffer_size) {
    BitReader reader(buffer + offset, 6);

    RCHECK(ReadBits(&reader, 16) == kAc3SyncWord);

    RCHECK(ReadBits(&reader, 2) != 3);

    reader.SkipBits(3);

    int frame_size = (ReadBits(&reader, 11) + 1) * 2;
    RCHECK(frame_size >= 7);

    reader.SkipBits(2 + 2 + 3 + 1);

    int bit_stream_id = ReadBits(&reader, 5);
    RCHECK(bit_stream_id >= 11 && bit_stream_id <= 16);

    offset += frame_size;
  }
  return true;
}
