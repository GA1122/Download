SplashError Splash::fillWithPattern(SplashPath *path, GBool eo,
				    SplashPattern *pattern,
				    SplashCoord alpha) {
  SplashPipe pipe;
  SplashXPath *xPath;
  SplashXPathScanner *scanner;
  int xMinI, yMinI, xMaxI, yMaxI, x0, x1, y;
  SplashClipResult clipRes, clipRes2;

  if (path->length == 0) {
    return splashErrEmptyPath;
  }
  if (pathAllOutside(path)) {
    opClipRes = splashClipAllOutside;
    return splashOk;
  }

  if (state->strokeAdjust && !path->hints) {
    int n;
    n = path->getLength();
    if (n == 4 &&
	!(path->flags[0] & splashPathClosed) &&
	!(path->flags[1] & splashPathLast) &&
	!(path->flags[2] & splashPathLast)) {
      path->close(gTrue);
      path->addStrokeAdjustHint(0, 2, 0, 4);
      path->addStrokeAdjustHint(1, 3, 0, 4);
    } else if (n == 5 &&
	       (path->flags[0] & splashPathClosed) &&
	       !(path->flags[1] & splashPathLast) &&
	       !(path->flags[2] & splashPathLast) &&
	       !(path->flags[3] & splashPathLast)) {
      path->addStrokeAdjustHint(0, 2, 0, 4);
      path->addStrokeAdjustHint(1, 3, 0, 4);
    }
  }

  xPath = new SplashXPath(path, state->matrix, state->flatness, gTrue);
  if (vectorAntialias && !inShading) {
    xPath->aaScale();
  }
  xPath->sort();
  yMinI = state->clip->getYMinI();
  yMaxI = state->clip->getYMaxI();
  if (vectorAntialias && !inShading) {
    yMinI = yMinI * splashAASize;
    yMaxI = (yMaxI + 1) * splashAASize - 1;
  }
  scanner = new SplashXPathScanner(xPath, eo, yMinI, yMaxI);

  if (vectorAntialias && !inShading) {
    scanner->getBBoxAA(&xMinI, &yMinI, &xMaxI, &yMaxI);
  } else {
    scanner->getBBox(&xMinI, &yMinI, &xMaxI, &yMaxI);
  }

  if ((clipRes = state->clip->testRect(xMinI, yMinI, xMaxI, yMaxI))
      != splashClipAllOutside) {
    if (scanner->hasPartialClip()) {
      clipRes = splashClipPartial;
    }

    pipeInit(&pipe, 0, yMinI, pattern, NULL, (Guchar)splashRound(alpha * 255),
	     vectorAntialias && !inShading, gFalse);

    if (vectorAntialias && !inShading) {
      for (y = yMinI; y <= yMaxI; ++y) {
	scanner->renderAALine(aaBuf, &x0, &x1, y);
	if (clipRes != splashClipAllInside) {
	  state->clip->clipAALine(aaBuf, &x0, &x1, y);
	}
	drawAALine(&pipe, x0, x1, y);
      }
    } else {
      for (y = yMinI; y <= yMaxI; ++y) {
	while (scanner->getNextSpan(y, &x0, &x1)) {
	  if (clipRes == splashClipAllInside) {
	    drawSpan(&pipe, x0, x1, y, gTrue);
	  } else {
	    if (x0 < state->clip->getXMinI()) {
	      x0 = state->clip->getXMinI();
	    }
	    if (x1 > state->clip->getXMaxI()) {
	      x1 = state->clip->getXMaxI();
	    }
	    clipRes2 = state->clip->testSpan(x0, x1, y);
	    drawSpan(&pipe, x0, x1, y, clipRes2 == splashClipAllInside);
	  }
	}
      }
    }
  }
  opClipRes = clipRes;

  delete scanner;
  delete xPath;
  return splashOk;
}
