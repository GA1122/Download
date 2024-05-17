static int GetMp3HeaderSize(const uint8* buffer, int buffer_size) {
  DCHECK_GE(buffer_size, 9);
  int size = ((buffer[6] & 0x7f) << 21) + ((buffer[7] & 0x7f) << 14) +
             ((buffer[8] & 0x7f) << 7) + (buffer[9] & 0x7f) + 10;
  if (buffer[5] & 0x10)   
    size += 10;
  return size;
}
