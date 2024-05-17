FoFiType1C *FoFiType1C::make(char *fileA, int lenA) {
  FoFiType1C *ff;

  ff = new FoFiType1C(fileA, lenA, gFalse);
  if (!ff->parse()) {
    delete ff;
    return NULL;
  }
  return ff;
}
