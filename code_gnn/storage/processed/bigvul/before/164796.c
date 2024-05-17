static bool CheckMpeg4BitStream(const uint8_t* buffer, int buffer_size) {
  RCHECK(buffer_size > 4);

  int offset = 0;
  int sequence_start_count = 0;
  int sequence_end_count = 0;
  int visual_object_count = 0;
  int vop_count = 0;
  while (true) {
    if (!AdvanceToStartCode(buffer, buffer_size, &offset, 6, 24, 1)) {
      return (sequence_start_count > 0 && visual_object_count > 0);
    }

    BitReader reader(buffer + offset, 6);
    RCHECK(ReadBits(&reader, 24) == 1);

    int start_code = ReadBits(&reader, 8);
    RCHECK(start_code < 0x30 || start_code > 0xaf);   
    RCHECK(start_code < 0xb7 || start_code > 0xb9);   

    switch (start_code) {
      case VISUAL_OBJECT_SEQUENCE_START_CODE: {
        ++sequence_start_count;
        int profile = ReadBits(&reader, 8);
        RCHECK(profile > 0);
        RCHECK(profile < 0x04 || profile > 0x10);
        RCHECK(profile < 0x13 || profile > 0x20);
        RCHECK(profile < 0x23 || profile > 0x31);
        RCHECK(profile < 0x35 || profile > 0x41);
        RCHECK(profile < 0x43 || profile > 0x60);
        RCHECK(profile < 0x65 || profile > 0x70);
        RCHECK(profile < 0x73 || profile > 0x80);
        RCHECK(profile < 0x83 || profile > 0x90);
        RCHECK(profile < 0x95 || profile > 0xa0);
        RCHECK(profile < 0xa4 || profile > 0xb0);
        RCHECK(profile < 0xb5 || profile > 0xc0);
        RCHECK(profile < 0xc3 || profile > 0xd0);
        RCHECK(profile < 0xe4);
        break;
      }

      case VISUAL_OBJECT_SEQUENCE_END_CODE:
        RCHECK(++sequence_end_count == sequence_start_count);
        break;

      case VISUAL_OBJECT_START_CODE: {
        ++visual_object_count;
        if (ReadBits(&reader, 1) == 1) {
          int visual_object_verid = ReadBits(&reader, 4);
          RCHECK(visual_object_verid > 0 && visual_object_verid < 3);
          RCHECK(ReadBits(&reader, 3) != 0);
        }
        int visual_object_type = ReadBits(&reader, 4);
        RCHECK(visual_object_type > 0 && visual_object_type < 6);
        break;
      }

      case VOP_START_CODE:
        RCHECK(++vop_count <= visual_object_count);
        break;
    }
    offset += 6;
  }
}
