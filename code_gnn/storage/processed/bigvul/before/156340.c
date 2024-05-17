static inline int DecodeNonASCIISequence(const uint8_t* sequence,
                                         unsigned length) {
  DCHECK(!IsASCII(sequence[0]));
  if (length == 2) {
    DCHECK_LE(sequence[0], 0xDF);
    if (sequence[0] < 0xC2)
      return kNonCharacter1;
    if (sequence[1] < 0x80 || sequence[1] > 0xBF)
      return kNonCharacter1;
    return ((sequence[0] << 6) + sequence[1]) - 0x00003080;
  }
  if (length == 3) {
    DCHECK_GE(sequence[0], 0xE0);
    DCHECK_LE(sequence[0], 0xEF);
    switch (sequence[0]) {
      case 0xE0:
        if (sequence[1] < 0xA0 || sequence[1] > 0xBF)
          return kNonCharacter1;
        break;
      case 0xED:
        if (sequence[1] < 0x80 || sequence[1] > 0x9F)
          return kNonCharacter1;
        break;
      default:
        if (sequence[1] < 0x80 || sequence[1] > 0xBF)
          return kNonCharacter1;
    }
    if (sequence[2] < 0x80 || sequence[2] > 0xBF)
      return kNonCharacter2;
    return ((sequence[0] << 12) + (sequence[1] << 6) + sequence[2]) -
           0x000E2080;
  }
  DCHECK_EQ(length, 4u);
  DCHECK_GE(sequence[0], 0xF0);
  DCHECK_LE(sequence[0], 0xF4);
  switch (sequence[0]) {
    case 0xF0:
      if (sequence[1] < 0x90 || sequence[1] > 0xBF)
        return kNonCharacter1;
      break;
    case 0xF4:
      if (sequence[1] < 0x80 || sequence[1] > 0x8F)
        return kNonCharacter1;
      break;
    default:
      if (sequence[1] < 0x80 || sequence[1] > 0xBF)
        return kNonCharacter1;
  }
  if (sequence[2] < 0x80 || sequence[2] > 0xBF)
    return kNonCharacter2;
  if (sequence[3] < 0x80 || sequence[3] > 0xBF)
    return kNonCharacter3;
  return ((sequence[0] << 18) + (sequence[1] << 12) + (sequence[2] << 6) +
          sequence[3]) -
         0x03C82080;
}
