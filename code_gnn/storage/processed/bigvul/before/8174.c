void Gfx::opXObject(Object args[], int numArgs) {
  char *name;
  Object obj1, obj2, obj3, refObj;
#if OPI_SUPPORT
  Object opiDict;
#endif

  name = args[0].getName();
  if (!res->lookupXObject(name, &obj1)) {
    return;
  }
  if (!obj1.isStream()) {
    error(getPos(), "XObject '%s' is wrong type", name);
    obj1.free();
    return;
  }

  obj1.streamGetDict()->lookupNF("OC", &obj2);
  if (obj2.isNull()) {
  } else if (obj2.isRef()) {
    if ( catalog->getOptContentConfig() && ! catalog->getOptContentConfig()->optContentIsVisible( &obj2 ) ) {
      obj2.free();
      obj1.free();
      return;
    }
  } else {
    error(getPos(), "XObject OC value not null or dict: %i", obj2.getType());
  }
  obj2.free();

#if OPI_SUPPORT
  obj1.streamGetDict()->lookup("OPI", &opiDict);
  if (opiDict.isDict()) {
    out->opiBegin(state, opiDict.getDict());
  }
#endif
  obj1.streamGetDict()->lookup("Subtype", &obj2);
  if (obj2.isName("Image")) {
    if (out->needNonText()) {
      res->lookupXObjectNF(name, &refObj);
      doImage(&refObj, obj1.getStream(), gFalse);
      refObj.free();
    }
  } else if (obj2.isName("Form")) {
    res->lookupXObjectNF(name, &refObj);
    if (out->useDrawForm() && refObj.isRef()) {
      out->drawForm(refObj.getRef());
    } else {
      doForm(&obj1);
    }
    refObj.free();
  } else if (obj2.isName("PS")) {
    obj1.streamGetDict()->lookup("Level1", &obj3);
    out->psXObject(obj1.getStream(),
		   obj3.isStream() ? obj3.getStream() : (Stream *)NULL);
  } else if (obj2.isName()) {
    error(getPos(), "Unknown XObject subtype '%s'", obj2.getName());
  } else {
    error(getPos(), "XObject subtype is missing or wrong type");
  }
  obj2.free();
#if OPI_SUPPORT
  if (opiDict.isDict()) {
    out->opiEnd(state, opiDict.getDict());
  }
  opiDict.free();
#endif
  obj1.free();
}
