void Gfx::opSetExtGState(Object args[], int numArgs) {
  Object obj1, obj2, obj3, obj4, obj5;
  GfxBlendMode mode;
  GBool haveFillOP;
  Function *funcs[4];
  GfxColor backdropColor;
  GBool haveBackdropColor;
  GfxColorSpace *blendingColorSpace;
  GBool alpha, isolated, knockout;
  int i;

  if (!res->lookupGState(args[0].getName(), &obj1)) {
    return;
  }
  if (!obj1.isDict()) {
    error(getPos(), "ExtGState '%s' is wrong type", args[0].getName());
    obj1.free();
    return;
  }
  if (printCommands) {
    printf("  gfx state dict: ");
    obj1.print();
    printf("\n");
  }

  if (!obj1.dictLookup("BM", &obj2)->isNull()) {
    if (state->parseBlendMode(&obj2, &mode)) {
      state->setBlendMode(mode);
      out->updateBlendMode(state);
    } else {
      error(getPos(), "Invalid blend mode in ExtGState");
    }
  }
  obj2.free();
  if (obj1.dictLookup("ca", &obj2)->isNum()) {
    state->setFillOpacity(obj2.getNum());
    out->updateFillOpacity(state);
  }
  obj2.free();
  if (obj1.dictLookup("CA", &obj2)->isNum()) {
    state->setStrokeOpacity(obj2.getNum());
    out->updateStrokeOpacity(state);
  }
  obj2.free();

  if ((haveFillOP = (obj1.dictLookup("op", &obj2)->isBool()))) {
    state->setFillOverprint(obj2.getBool());
    out->updateFillOverprint(state);
  }
  obj2.free();
  if (obj1.dictLookup("OP", &obj2)->isBool()) {
    state->setStrokeOverprint(obj2.getBool());
    out->updateStrokeOverprint(state);
    if (!haveFillOP) {
      state->setFillOverprint(obj2.getBool());
      out->updateFillOverprint(state);
    }
  }
  obj2.free();

  if (obj1.dictLookup("SA", &obj2)->isBool()) {
    state->setStrokeAdjust(obj2.getBool());
    out->updateStrokeAdjust(state);
  }
  obj2.free();

  if (obj1.dictLookup("TR2", &obj2)->isNull()) {
    obj2.free();
    obj1.dictLookup("TR", &obj2);
  }
  if (obj2.isName("Default") ||
      obj2.isName("Identity")) {
    funcs[0] = funcs[1] = funcs[2] = funcs[3] = NULL;
    state->setTransfer(funcs);
    out->updateTransfer(state);
  } else if (obj2.isArray() && obj2.arrayGetLength() == 4) {
    for (i = 0; i < 4; ++i) {
      obj2.arrayGet(i, &obj3);
      funcs[i] = Function::parse(&obj3);
      obj3.free();
      if (!funcs[i]) {
	break;
      }
    }
    if (i == 4) {
      state->setTransfer(funcs);
      out->updateTransfer(state);
    }
  } else if (obj2.isName() || obj2.isDict() || obj2.isStream()) {
    if ((funcs[0] = Function::parse(&obj2))) {
      funcs[1] = funcs[2] = funcs[3] = NULL;
      state->setTransfer(funcs);
      out->updateTransfer(state);
    }
  } else if (!obj2.isNull()) {
    error(getPos(), "Invalid transfer function in ExtGState");
  }
  obj2.free();

  if (obj1.dictLookup("AIS", &obj2)->isBool()) {
    state->setAlphaIsShape(obj2.getBool());
    out->updateAlphaIsShape(state);
  }
  obj2.free();

  if (obj1.dictLookup("TK", &obj2)->isBool()) {
    state->setTextKnockout(obj2.getBool());
    out->updateTextKnockout(state);
  }
  obj2.free();

  if (!obj1.dictLookup("SMask", &obj2)->isNull()) {
    if (obj2.isName("None")) {
      out->clearSoftMask(state);
    } else if (obj2.isDict()) {
      if (obj2.dictLookup("S", &obj3)->isName("Alpha")) {
	alpha = gTrue;
      } else {  
	alpha = gFalse;
      }
      obj3.free();
      funcs[0] = NULL;
      if (!obj2.dictLookup("TR", &obj3)->isNull()) {
	funcs[0] = Function::parse(&obj3);
	if (funcs[0]->getInputSize() != 1 ||
	    funcs[0]->getOutputSize() != 1) {
	  error(getPos(),
		"Invalid transfer function in soft mask in ExtGState");
	  delete funcs[0];
	  funcs[0] = NULL;
	}
      }
      obj3.free();
      if ((haveBackdropColor = obj2.dictLookup("BC", &obj3)->isArray())) {
	for (i = 0; i < gfxColorMaxComps; ++i) {
	  backdropColor.c[i] = 0;
	}
	for (i = 0; i < obj3.arrayGetLength() && i < gfxColorMaxComps; ++i) {
	  obj3.arrayGet(i, &obj4);
	  if (obj4.isNum()) {
	    backdropColor.c[i] = dblToCol(obj4.getNum());
	  }
	  obj4.free();
	}
      }
      obj3.free();
      if (obj2.dictLookup("G", &obj3)->isStream()) {
	if (obj3.streamGetDict()->lookup("Group", &obj4)->isDict()) {
	  blendingColorSpace = NULL;
	  isolated = knockout = gFalse;
	  if (!obj4.dictLookup("CS", &obj5)->isNull()) {
	    blendingColorSpace = GfxColorSpace::parse(&obj5, this);
	  }
	  obj5.free();
	  if (obj4.dictLookup("I", &obj5)->isBool()) {
	    isolated = obj5.getBool();
	  }
	  obj5.free();
	  if (obj4.dictLookup("K", &obj5)->isBool()) {
	    knockout = obj5.getBool();
	  }
	  obj5.free();
	  if (!haveBackdropColor) {
	    if (blendingColorSpace) {
	      blendingColorSpace->getDefaultColor(&backdropColor);
	    } else {
	      for (i = 0; i < gfxColorMaxComps; ++i) {
		backdropColor.c[i] = 0;
	      }
	    }
	  }
	  doSoftMask(&obj3, alpha, blendingColorSpace,
		     isolated, knockout, funcs[0], &backdropColor);
	  if (funcs[0]) {
	    delete funcs[0];
	  }
	} else {
	  error(getPos(), "Invalid soft mask in ExtGState - missing group");
	}
	obj4.free();
      } else {
	error(getPos(), "Invalid soft mask in ExtGState - missing group");
      }
      obj3.free();
    } else if (!obj2.isNull()) {
      error(getPos(), "Invalid soft mask in ExtGState");
    }
  }
  obj2.free();
  if (obj1.dictLookup("Font", &obj2)->isArray()) {
    GfxFont *font;
    if (obj2.arrayGetLength() == 2) {
      Object fargs0, fargs1;

      obj2.arrayGetNF(0,&fargs0);
      obj2.arrayGet(1,&fargs1);
      if (fargs0.isRef() && fargs1.isNum()) {
	Object fobj;
	Ref r;

	fargs0.fetch(xref, &fobj);
	if (fobj.isDict()) {
	  r = fargs0.getRef();
	  font = GfxFont::makeFont(xref,args[0].getName(),r,fobj.getDict());
	  state->setFont(font,fargs1.getNum());
	  fontChanged = gTrue;
	}
	fobj.free();
      }
      fargs0.free();
      fargs1.free();
    } else {
      error(getPos(), "Number of args mismatch for /Font in ExtGState");
    }
  }
  obj2.free();
  if (obj1.dictLookup("LW", &obj2)->isNum()) {
    opSetLineWidth(&obj2,1);
  }
  obj2.free();
  if (obj1.dictLookup("LC", &obj2)->isInt()) {
    opSetLineCap(&obj2,1);
  }
  obj2.free();
  if (obj1.dictLookup("LJ", &obj2)->isInt()) {
    opSetLineJoin(&obj2,1);
  }
  obj2.free();
  if (obj1.dictLookup("ML", &obj2)->isNum()) {
    opSetMiterLimit(&obj2,1);
  }
  obj2.free();
  if (obj1.dictLookup("D", &obj2)->isArray()) {
    if (obj2.arrayGetLength() == 2) {
      Object dargs[2];

      obj2.arrayGetNF(0,&dargs[0]);
      obj2.arrayGet(1,&dargs[1]);
      if (dargs[0].isArray() && dargs[1].isInt()) {
	opSetDash(dargs,2);
      }
      dargs[0].free();
      dargs[1].free();
    } else {
      error(getPos(), "Number of args mismatch for /D in ExtGState");
    }
  }
  obj2.free();
  if (obj1.dictLookup("RI", &obj2)->isName()) {
    opSetRenderingIntent(&obj2,1);
  }
  obj2.free();
  if (obj1.dictLookup("FL", &obj2)->isNum()) {
    opSetFlat(&obj2,1);
  }
  obj2.free();

  obj1.free();
}
