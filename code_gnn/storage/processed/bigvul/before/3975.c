short CCITTFaxStream::getTwoDimCode() {
  int code;
  const CCITTCode *p;
  int n;

  code = 0;  
  if (endOfBlock) {
    if ((code = lookBits(7)) != EOF) {
      p = &twoDimTab1[code];
      if (p->bits > 0) {
	eatBits(p->bits);
	return p->n;
      }
    }
  } else {
    for (n = 1; n <= 7; ++n) {
      if ((code = lookBits(n)) == EOF) {
	break;
      }
      if (n < 7) {
	code <<= 7 - n;
      }
      p = &twoDimTab1[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
  }
  error(errSyntaxError, getPos(),
	"Bad two dim code ({0:04x}) in CCITTFax stream", code);
  return EOF;
}
