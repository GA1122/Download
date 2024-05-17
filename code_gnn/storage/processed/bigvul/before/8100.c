GBool GfxResources::lookupGStateNF(char *name, Object *obj) {
  GfxResources *resPtr;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->gStateDict.isDict()) {
      if (!resPtr->gStateDict.dictLookupNF(name, obj)->isNull()) {
	return gTrue;
      }
      obj->free();
    }
  }
  error(-1, "ExtGState '%s' is unknown", name);
  return gFalse;
}
