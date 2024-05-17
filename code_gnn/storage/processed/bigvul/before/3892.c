ASCIIHexStream::ASCIIHexStream(Stream *strA):
    FilterStream(strA) {
  buf = EOF;
  eof = gFalse;
}
