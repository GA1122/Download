void RunLengthStream::reset() {
  str->reset();
  bufPtr = bufEnd = buf;
  eof = gFalse;
}
