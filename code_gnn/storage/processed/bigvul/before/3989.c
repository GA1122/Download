int ASCIIHexStream::lookChar() {
  int c1, c2, x;

  if (buf != EOF)
    return buf;
  if (eof) {
    buf = EOF;
    return EOF;
  }
  do {
    c1 = str->getChar();
  } while (isspace(c1));
  if (c1 == '>') {
    eof = gTrue;
    buf = EOF;
    return buf;
  }
  do {
    c2 = str->getChar();
  } while (isspace(c2));
  if (c2 == '>') {
    eof = gTrue;
    c2 = '0';
  }
  if (c1 >= '0' && c1 <= '9') {
    x = (c1 - '0') << 4;
  } else if (c1 >= 'A' && c1 <= 'F') {
    x = (c1 - 'A' + 10) << 4;
  } else if (c1 >= 'a' && c1 <= 'f') {
    x = (c1 - 'a' + 10) << 4;
  } else if (c1 == EOF) {
    eof = gTrue;
    x = 0;
  } else {
    error(errSyntaxError, getPos(),
	  "Illegal character <{0:02x}> in ASCIIHex stream", c1);
    x = 0;
  }
  if (c2 >= '0' && c2 <= '9') {
    x += c2 - '0';
  } else if (c2 >= 'A' && c2 <= 'F') {
    x += c2 - 'A' + 10;
  } else if (c2 >= 'a' && c2 <= 'f') {
    x += c2 - 'a' + 10;
  } else if (c2 == EOF) {
    eof = gTrue;
    x = 0;
  } else {
    error(errSyntaxError, getPos(),
	  "Illegal character <{0:02x}> in ASCIIHex stream", c2);
  }
  buf = x & 0xff;
  return buf;
}
