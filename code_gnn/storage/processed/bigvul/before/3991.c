int LZWStream::lookChar() {
  if (pred) {
    return pred->lookChar();
  }
  if (eof) {
    return EOF;
  }
  if (seqIndex >= seqLength) {
    if (!processNextCode()) {
      return EOF;
    }
  }
  return seqBuf[seqIndex];
}
