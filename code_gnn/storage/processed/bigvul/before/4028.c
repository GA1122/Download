GBool DCTStream::readTrailer() {
  int c;

  c = readMarker();
  if (c != 0xd9) {		 
    error(errSyntaxError, getPos(), "Bad DCT trailer");
    return gFalse;
  }
  return gTrue;
}