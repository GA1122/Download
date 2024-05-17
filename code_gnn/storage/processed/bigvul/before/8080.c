void Gfx::doImage(Object *ref, Stream *str, GBool inlineImg) {
  Dict *dict, *maskDict;
  int width, height;
  int bits, maskBits;
  GBool interpolate;
  StreamColorSpaceMode csMode;
  GBool mask;
  GBool invert;
  GfxColorSpace *colorSpace, *maskColorSpace;
  GfxImageColorMap *colorMap, *maskColorMap;
  Object maskObj, smaskObj;
  GBool haveColorKeyMask, haveExplicitMask, haveSoftMask;
  int maskColors[2*gfxColorMaxComps];
  int maskWidth, maskHeight;
  GBool maskInvert;
  GBool maskInterpolate;
  Stream *maskStr;
  Object obj1, obj2;
  int i;

  bits = 0;
  csMode = streamCSNone;
  str->getImageParams(&bits, &csMode);

  dict = str->getDict();

  dict->lookup("Width", &obj1);
  if (obj1.isNull()) {
    obj1.free();
    dict->lookup("W", &obj1);
  }
  if (obj1.isInt())
    width = obj1.getInt();
  else if (obj1.isReal())
    width = (int)obj1.getReal();
  else
    goto err2;
  obj1.free();
  dict->lookup("Height", &obj1);
  if (obj1.isNull()) {
    obj1.free();
    dict->lookup("H", &obj1);
  }
  if (obj1.isInt())
    height = obj1.getInt();
  else if (obj1.isReal())
    height = (int)obj1.getReal();
  else
    goto err2;
  obj1.free();

  if (width < 1 || height < 1)
    goto err1;

  dict->lookup("Interpolate", &obj1);
  if (obj1.isNull()) {
    obj1.free();
    dict->lookup("I", &obj1);
  }
  if (obj1.isBool())
    interpolate = obj1.getBool();
  else
    interpolate = gFalse;
  obj1.free();
  maskInterpolate = gFalse;

  dict->lookup("ImageMask", &obj1);
  if (obj1.isNull()) {
    obj1.free();
    dict->lookup("IM", &obj1);
  }
  mask = gFalse;
  if (obj1.isBool())
    mask = obj1.getBool();
  else if (!obj1.isNull())
    goto err2;
  obj1.free();

  if (bits == 0) {
    dict->lookup("BitsPerComponent", &obj1);
    if (obj1.isNull()) {
      obj1.free();
      dict->lookup("BPC", &obj1);
    }
    if (obj1.isInt()) {
      bits = obj1.getInt();
    } else if (mask) {
      bits = 1;
    } else {
      goto err2;
    }
    obj1.free();
  }

  if (mask) {

    if (bits != 1)
      goto err1;
    invert = gFalse;
    dict->lookup("Decode", &obj1);
    if (obj1.isNull()) {
      obj1.free();
      dict->lookup("D", &obj1);
    }
    if (obj1.isArray()) {
      obj1.arrayGet(0, &obj2);
      if (obj2.isNum() && obj2.getNum() >= 0.9)
	invert = gTrue;
      obj2.free();
    } else if (!obj1.isNull()) {
      goto err2;
    }
    obj1.free();

    if (!contentIsHidden()) {
	out->drawImageMask(state, ref, str, width, height, invert, interpolate, inlineImg);
      if (out->fillMaskCSPattern(state)) {
        maskHaveCSPattern = gTrue;
        doPatternFill(gTrue);
        out->endMaskClip(state);
        maskHaveCSPattern = gFalse;
      }
    }
  } else {

    dict->lookup("ColorSpace", &obj1);
    if (obj1.isNull()) {
      obj1.free();
      dict->lookup("CS", &obj1);
    }
    if (obj1.isName() && inlineImg) {
      res->lookupColorSpace(obj1.getName(), &obj2);
      if (!obj2.isNull()) {
	obj1.free();
	obj1 = obj2;
      } else {
	obj2.free();
      }
    }
    if (!obj1.isNull()) {
      colorSpace = GfxColorSpace::parse(&obj1, this);
    } else if (csMode == streamCSDeviceGray) {
      colorSpace = new GfxDeviceGrayColorSpace();
    } else if (csMode == streamCSDeviceRGB) {
      colorSpace = new GfxDeviceRGBColorSpace();
    } else if (csMode == streamCSDeviceCMYK) {
      colorSpace = new GfxDeviceCMYKColorSpace();
    } else {
      colorSpace = NULL;
    }
    obj1.free();
    if (!colorSpace) {
      goto err1;
    }
    dict->lookup("Decode", &obj1);
    if (obj1.isNull()) {
      obj1.free();
      dict->lookup("D", &obj1);
    }
    colorMap = new GfxImageColorMap(bits, &obj1, colorSpace);
    obj1.free();
    if (!colorMap->isOk()) {
      delete colorMap;
      goto err1;
    }

    haveColorKeyMask = haveExplicitMask = haveSoftMask = gFalse;
    maskStr = NULL;  
    maskWidth = maskHeight = 0;  
    maskInvert = gFalse;  
    maskColorMap = NULL;  
    dict->lookup("Mask", &maskObj);
    dict->lookup("SMask", &smaskObj);
    if (smaskObj.isStream()) {
      if (inlineImg) {
	goto err1;
      }
      maskStr = smaskObj.getStream();
      maskDict = smaskObj.streamGetDict();
      maskDict->lookup("Width", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("W", &obj1);
      }
      if (!obj1.isInt()) {
	goto err2;
      }
      maskWidth = obj1.getInt();
      obj1.free();
      maskDict->lookup("Height", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("H", &obj1);
      }
      if (!obj1.isInt()) {
	goto err2;
      }
      maskHeight = obj1.getInt();
      obj1.free();
      maskDict->lookup("Interpolate", &obj1);
      if (obj1.isNull()) {
        obj1.free();
        maskDict->lookup("I", &obj1);
      }
      if (obj1.isBool())
        maskInterpolate = obj1.getBool();
      else
        maskInterpolate = gFalse;
      obj1.free();
      maskDict->lookup("BitsPerComponent", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("BPC", &obj1);
      }
      if (!obj1.isInt()) {
	goto err2;
      }
      maskBits = obj1.getInt();
      obj1.free();
      maskDict->lookup("ColorSpace", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("CS", &obj1);
      }
      if (obj1.isName()) {
	res->lookupColorSpace(obj1.getName(), &obj2);
	if (!obj2.isNull()) {
	  obj1.free();
	  obj1 = obj2;
	} else {
	  obj2.free();
	}
      }
      maskColorSpace = GfxColorSpace::parse(&obj1, this);
      obj1.free();
      if (!maskColorSpace || maskColorSpace->getMode() != csDeviceGray) {
	goto err1;
      }
      maskDict->lookup("Decode", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("D", &obj1);
      }
      maskColorMap = new GfxImageColorMap(maskBits, &obj1, maskColorSpace);
      obj1.free();
      if (!maskColorMap->isOk()) {
	delete maskColorMap;
	goto err1;
      }
      haveSoftMask = gTrue;
    } else if (maskObj.isArray()) {
      for (i = 0;
	   i < maskObj.arrayGetLength() && i < 2*gfxColorMaxComps;
	   ++i) {
	maskObj.arrayGet(i, &obj1);
	if (obj1.isInt()) {
	  maskColors[i] = obj1.getInt();
	} else if (obj1.isReal()) {
	  error(-1, "Mask entry should be an integer but it's a real, trying to use it");
	  maskColors[i] = (int) obj1.getReal();
	} else {
	  error(-1, "Mask entry should be an integer but it's of type %d", obj1.getType());
	  obj1.free();
	  goto err1;
	}
	obj1.free();
      }
      haveColorKeyMask = gTrue;
    } else if (maskObj.isStream()) {
      if (inlineImg) {
	goto err1;
      }
      maskStr = maskObj.getStream();
      maskDict = maskObj.streamGetDict();
      maskDict->lookup("Width", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("W", &obj1);
      }
      if (!obj1.isInt()) {
	goto err2;
      }
      maskWidth = obj1.getInt();
      obj1.free();
      maskDict->lookup("Height", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("H", &obj1);
      }
      if (!obj1.isInt()) {
	goto err2;
      }
      maskHeight = obj1.getInt();
      obj1.free();
      maskDict->lookup("Interpolate", &obj1);
      if (obj1.isNull()) {
        obj1.free();
	maskDict->lookup("I", &obj1);
      }
      if (obj1.isBool())
        maskInterpolate = obj1.getBool();
      else
        maskInterpolate = gFalse;
      obj1.free();
      maskDict->lookup("ImageMask", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("IM", &obj1);
      }
      if (!obj1.isBool() || !obj1.getBool()) {
	goto err2;
      }
      obj1.free();
      maskInvert = gFalse;
      maskDict->lookup("Decode", &obj1);
      if (obj1.isNull()) {
	obj1.free();
	maskDict->lookup("D", &obj1);
      }
      if (obj1.isArray()) {
	obj1.arrayGet(0, &obj2);
	if (obj2.isNum() && obj2.getNum() >= 0.9) {
	  maskInvert = gTrue;
	}
	obj2.free();
      } else if (!obj1.isNull()) {
	goto err2;
      }
      obj1.free();
      haveExplicitMask = gTrue;
    }

    if (haveSoftMask) {
      if (!contentIsHidden()) {
        out->drawSoftMaskedImage(state, ref, str, width, height, colorMap, interpolate,
				 maskStr, maskWidth, maskHeight, maskColorMap, maskInterpolate);
      }
      delete maskColorMap;
    } else if (haveExplicitMask && !contentIsHidden ()) {
      out->drawMaskedImage(state, ref, str, width, height, colorMap, interpolate,
			   maskStr, maskWidth, maskHeight, maskInvert, maskInterpolate);
    } else if (!contentIsHidden()) {
      out->drawImage(state, ref, str, width, height, colorMap, interpolate,
		     haveColorKeyMask ? maskColors : (int *)NULL, inlineImg);
    }
    delete colorMap;

    maskObj.free();
    smaskObj.free();
  }

  if ((i = width * height) > 1000) {
    i = 1000;
  }
  updateLevel += i;

  return;

 err2:
  obj1.free();
 err1:
  error(getPos(), "Bad image parameters");
}
