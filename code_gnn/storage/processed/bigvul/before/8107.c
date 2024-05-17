void Gfx::opBeginImage(Object args[], int numArgs) {
  Stream *str;
  int c1, c2;

  str = buildImageStream();

  if (str) {
    doImage(NULL, str, gTrue);
  
    c1 = str->getUndecodedStream()->getChar();
    c2 = str->getUndecodedStream()->getChar();
    while (!(c1 == 'E' && c2 == 'I') && c2 != EOF) {
      c1 = c2;
      c2 = str->getUndecodedStream()->getChar();
    }
    delete str;
  }
}
