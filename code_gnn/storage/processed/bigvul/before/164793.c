static bool CheckMp3(const uint8_t* buffer, int buffer_size) {
  int numSeen = 0;
  int offset = 0;

  while (offset < buffer_size && buffer[offset] == 0)
    ++offset;

  while (offset + 3 < buffer_size) {
    int framesize;
    RCHECK(ValidMpegAudioFrameHeader(
        buffer + offset, buffer_size - offset, &framesize));

    if (++numSeen > 10)
      return true;
    offset += framesize;
  }
  return numSeen > 2;
}
