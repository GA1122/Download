GfxShading *GfxResources::lookupShading(char *name, Gfx *gfx) {
  GfxResources *resPtr;
  GfxShading *shading;
  Object obj;

  for (resPtr = this; resPtr; resPtr = resPtr->next) {
    if (resPtr->shadingDict.isDict()) {
      if (!resPtr->shadingDict.dictLookup(name, &obj)->isNull()) {
	shading = GfxShading::parse(&obj, gfx);
	obj.free();
	return shading;
      }
      obj.free();
    }
  }
  error(-1, "Unknown shading '%s'", name);
  return NULL;
}
