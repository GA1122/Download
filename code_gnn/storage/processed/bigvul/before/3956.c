char *Stream::getLine(char *buf, int size) {
  int i;
  int c;

  if (lookChar() == EOF || size < 0)
    return NULL;
  for (i = 0; i < size - 1; ++i) {
    c = getChar();
    if (c == EOF || c == '\n')
      break;
    if (c == '\r') {
      if ((c = lookChar()) == '\n')
	getChar();
      break;
    }
    buf[i] = c;
  }
  buf[i] = '\0';
  return buf;
}
