int StreamPredictor::getChar() {
  if (predIdx >= rowBytes) {
    if (!getNextLine()) {
      return EOF;
    }
  }
  return predLine[predIdx++];
}
