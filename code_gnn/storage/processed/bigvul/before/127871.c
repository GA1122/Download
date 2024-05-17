static bool CheckWebm(const uint8* buffer, int buffer_size) {
  RCHECK(buffer_size > 12);

  BitReader reader(buffer, buffer_size);

  RCHECK(GetElementId(&reader) == 0x1a45dfa3);

  int header_size = GetVint(&reader);
  RCHECK(reader.bits_available() / 8 >= header_size);

  while (reader.bits_available() > 0) {
    int tag = GetElementId(&reader);
    int tagsize = GetVint(&reader);
    switch (tag) {
      case 0x4286:   
      case 0x42f7:   
      case 0x42f2:   
      case 0x42f3:   
      case 0x4287:   
      case 0x4285:   
      case 0xec:     
      case 0xbf:     
        RCHECK(reader.SkipBits(tagsize * 8));
        break;

      case 0x4282:   
        switch (ReadBits(&reader, 32)) {
          case TAG('w', 'e', 'b', 'm') :
            return true;
          case TAG('m', 'a', 't', 'r') :
            return (ReadBits(&reader, 32) == TAG('o', 's', 'k', 'a'));
        }
        return false;

      default:   
        return false;
    }
  }
  return false;
}
