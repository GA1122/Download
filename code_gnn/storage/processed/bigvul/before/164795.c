static bool CheckMpeg2TransportStream(const uint8_t* buffer, int buffer_size) {
  RCHECK(buffer_size >= 250);   

  int offset = 0;
  int packet_length = -1;
  while (buffer[offset] != kMpeg2SyncWord && offset < 20) {
    ++offset;
  }

  while (offset + 6 < buffer_size) {
    BitReader reader(buffer + offset, 6);

    RCHECK(ReadBits(&reader, 8) == kMpeg2SyncWord);

    reader.SkipBits(1 + 1 + 1);

    int pid = ReadBits(&reader, 13);
    RCHECK(pid < 3 || pid > 15);

    reader.SkipBits(2);

    int adaptation_field_control = ReadBits(&reader, 2);
    RCHECK(adaptation_field_control != 0);

    if (adaptation_field_control >= 2) {
      reader.SkipBits(4);

      int adaptation_field_length = ReadBits(&reader, 8);
      if (adaptation_field_control == 2)
        RCHECK(adaptation_field_length == 183);
      else
        RCHECK(adaptation_field_length <= 182);
    }

    if (packet_length < 0) {
      if (buffer[offset + 188] == kMpeg2SyncWord)
        packet_length = 188;
      else if (buffer[offset + 192] == kMpeg2SyncWord)
        packet_length = 192;
      else if (buffer[offset + 204] == kMpeg2SyncWord)
        packet_length = 204;
      else
        packet_length = 208;
    }
    offset += packet_length;
  }
  return true;
}
