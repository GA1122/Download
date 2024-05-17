int FlateStream::getCodeWord(int bits) {
  int c;

  while (codeSize < bits) {
    if ((c = str->getChar()) == EOF)
      return EOF;
    codeBuf |= (c & 0xff) << codeSize;
    codeSize += 8;
  }
  c = codeBuf & ((1 << bits) - 1);
  codeBuf >>= bits;
  codeSize -= bits;
  return c;
}
