GfxResources::GfxResources(XRef *xref, Dict *resDict, GfxResources *nextA) :
    gStateCache(2, xref) {
  Object obj1, obj2;
  Ref r;

  if (resDict) {

    fonts = NULL;
    resDict->lookupNF("Font", &obj1);
    if (obj1.isRef()) {
      obj1.fetch(xref, &obj2);
      if (obj2.isDict()) {
	r = obj1.getRef();
	fonts = new GfxFontDict(xref, &r, obj2.getDict());
      }
      obj2.free();
    } else if (obj1.isDict()) {
      fonts = new GfxFontDict(xref, NULL, obj1.getDict());
    }
    obj1.free();

    resDict->lookup("XObject", &xObjDict);

    resDict->lookup("ColorSpace", &colorSpaceDict);

    resDict->lookup("Pattern", &patternDict);

    resDict->lookup("Shading", &shadingDict);

    resDict->lookup("ExtGState", &gStateDict);

    resDict->lookup("Properties", &propertiesDict);

  } else {
    fonts = NULL;
    xObjDict.initNull();
    colorSpaceDict.initNull();
    patternDict.initNull();
    shadingDict.initNull();
    gStateDict.initNull();
    propertiesDict.initNull();
  }

  next = nextA;
}
