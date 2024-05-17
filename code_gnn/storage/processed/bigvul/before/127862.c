static bool CheckH263(const uint8* buffer, int buffer_size) {
  RCHECK(buffer_size > 16);

  int offset = 0;
  bool seen_start_code = false;
  while (true) {
    if (!AdvanceToStartCode(buffer, buffer_size, &offset, 9, 22, 0x20)) {
      return seen_start_code;
    }

    BitReader reader(buffer + offset, 9);
    RCHECK(ReadBits(&reader, 22) == 0x20);

    reader.SkipBits(8);

    RCHECK(ReadBits(&reader, 2) == 2);

    reader.SkipBits(1 + 1 + 1);

    int format = ReadBits(&reader, 3);
    RCHECK(format != 0 && format != 6);   

    if (format == 7) {
      int ufep = ReadBits(&reader, 3);
      if (ufep == 1) {
        format = ReadBits(&reader, 3);
        RCHECK(format != 0 && format != 7);   
        reader.SkipBits(11);
        RCHECK(ReadBits(&reader, 4) == 8);   
      } else {
        RCHECK(ufep == 0);   
      }

      int picture_type_code = ReadBits(&reader, 3);
      RCHECK(picture_type_code != 6 && picture_type_code != 7);   

      reader.SkipBits(1 + 1 + 1);

      RCHECK(ReadBits(&reader, 3) == 1);   
    }

    seen_start_code = true;
    offset += 9;
  }
}
