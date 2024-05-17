static bool CheckMpeg2ProgramStream(const uint8_t* buffer, int buffer_size) {
  RCHECK(buffer_size > 14);

  int offset = 0;
  while (offset + 14 < buffer_size) {
    BitReader reader(buffer + offset, 14);

    RCHECK(ReadBits(&reader, 24) == 1);
    RCHECK(ReadBits(&reader, 8) == PACK_START_CODE);

    int mpeg_version = ReadBits(&reader, 2);
    if (mpeg_version == 0) {
      RCHECK(ReadBits(&reader, 2) == 2);
    } else {
      RCHECK(mpeg_version == 1);
    }

    reader.SkipBits(3);

    RCHECK(ReadBits(&reader, 1) == 1);

    reader.SkipBits(15);

    RCHECK(ReadBits(&reader, 1) == 1);

    reader.SkipBits(15);

    RCHECK(ReadBits(&reader, 1) == 1);

    if (mpeg_version == 0) {
      RCHECK(ReadBits(&reader, 1) == 1);

      reader.SkipBits(22);

      RCHECK(ReadBits(&reader, 1) == 1);

      offset += 12;
    } else {
      reader.SkipBits(22);

      RCHECK(ReadBits(&reader, 2) == 3);

      reader.SkipBits(5);

      int pack_stuffing_length = ReadBits(&reader, 3);
      offset += 14 + pack_stuffing_length;
    }

    while (offset + 6 < buffer_size && Read24(buffer + offset) == 1) {
      int stream_id = buffer[offset + 3];

      if (mpeg_version == 0)
        RCHECK(stream_id != 0xbc && stream_id < 0xf0);
      else
        RCHECK(stream_id != 0xfc && stream_id != 0xfd && stream_id != 0xfe);

      if (stream_id == PACK_START_CODE)   
        break;
      if (stream_id == PROGRAM_END_CODE)   
        return true;

      int pes_length = Read16(buffer + offset + 4);
      RCHECK(pes_length > 0);
      offset = offset + 6 + pes_length;
    }
  }
  return true;
}
