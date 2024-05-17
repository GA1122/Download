Guint JBIG2MMRDecoder::get24Bits() {
  while (bufLen < 24) {
    buf = (buf << 8) | (str->getChar() & 0xff);
    bufLen += 8;
    ++nBytesRead;
  }
  return (buf >> (bufLen - 24)) & 0xffffff;
}
