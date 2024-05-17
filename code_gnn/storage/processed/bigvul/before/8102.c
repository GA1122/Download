GfxPattern *GfxResources::lookupPattern(char *name, Gfx *gfx) {
  GfxResources *resPtr;
  GfxPattern *pattern;
  Object obj;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->patternDict.isDict()) {
      if (!resPtr->patternDict.dictLookup(name, &obj)->isNull()) {
	pattern = GfxPattern::parse(&obj, gfx);
	obj.free();
	return pattern;
      }
      obj.free();
    }
  }
  error(-1, "Unknown pattern '%s'", name);
  return NULL;
}
