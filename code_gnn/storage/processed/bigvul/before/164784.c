static bool CheckDV(const uint8_t* buffer, int buffer_size) {
  RCHECK(buffer_size > 11);

  int offset = 0;
  int current_sequence_number = -1;
  int last_block_number[6] = {0};
  while (offset + 11 < buffer_size) {
    BitReader reader(buffer + offset, 11);

    int section = ReadBits(&reader, 3);
    RCHECK(section < 5);

    RCHECK(ReadBits(&reader, 1) == 1);

    reader.SkipBits(4);

    int sequence_number = ReadBits(&reader, 4);

    reader.SkipBits(1);

    RCHECK(ReadBits(&reader, 3) == 7);

    int block_number = ReadBits(&reader, 8);

    if (section == 0) {   
      reader.SkipBits(1);
      RCHECK(ReadBits(&reader, 1) == 0);
      RCHECK(ReadBits(&reader, 11) == 0x7ff);
      reader.SkipBits(4);
      RCHECK(ReadBits(&reader, 4) == 0xf);
      reader.SkipBits(4);
      RCHECK(ReadBits(&reader, 4) == 0xf);
      reader.SkipBits(4);
      RCHECK(ReadBits(&reader, 4) == 0xf);
      reader.SkipBits(3);
      RCHECK(ReadBits(&reader, 24) == 0xffffff);
      current_sequence_number = sequence_number;
      for (size_t i = 0; i < base::size(last_block_number); ++i)
        last_block_number[i] = -1;
    } else {
      RCHECK(sequence_number == current_sequence_number);
      RCHECK(block_number > last_block_number[section]);
      last_block_number[section] = block_number;
    }

    offset += 80;
  }
  return true;
}
