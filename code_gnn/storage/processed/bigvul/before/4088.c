void Splash::blitImage(SplashBitmap *src, GBool srcAlpha, int xDest, int yDest,
		       SplashClipResult clipRes) {
  SplashPipe pipe;
  SplashColor pixel;
  Guchar *ap;
  int w, h, x0, y0, x1, y1, x, y;

  w = src->getWidth();
  h = src->getHeight();
  if (clipRes == splashClipAllInside) {
    x0 = 0;
    y0 = 0;
    x1 = w;
    y1 = h;
  } else {
    if (state->clip->getNumPaths()) {
      x0 = x1 = w;
      y0 = y1 = h;
    } else {
      if ((x0 = splashCeil(state->clip->getXMin()) - xDest) < 0) {
	x0 = 0;
      }
      if ((y0 = splashCeil(state->clip->getYMin()) - yDest) < 0) {
	y0 = 0;
      }
      if ((x1 = splashFloor(state->clip->getXMax()) - xDest) > w) {
	x1 = w;
      }
      if (x1 < x0) {
	x1 = x0;
      }
      if ((y1 = splashFloor(state->clip->getYMax()) - yDest) > h) {
	y1 = h;
      }
      if (y1 < y0) {
	y1 = y0;
      }
    }
  }

  if (x0 < w && y0 < h && x0 < x1 && y0 < y1) {
    pipeInit(&pipe, xDest + x0, yDest + y0, NULL, pixel,
	     (Guchar)splashRound(state->fillAlpha * 255), srcAlpha, gFalse);
    if (srcAlpha) {
      for (y = y0; y < y1; ++y) {
	pipeSetXY(&pipe, xDest + x0, yDest + y);
	ap = src->getAlphaPtr() + y * w + x0;
	for (x = x0; x < x1; ++x) {
	  src->getPixel(x, y, pixel);
	  pipe.shape = *ap++;
	  (this->*pipe.run)(&pipe);
	}
      }
    } else {
      for (y = y0; y < y1; ++y) {
	pipeSetXY(&pipe, xDest + x0, yDest + y);
	for (x = x0; x < x1; ++x) {
	  src->getPixel(x, y, pixel);
	  (this->*pipe.run)(&pipe);
	}
      }
    }
    updateModX(xDest + x0);
    updateModX(xDest + x1 - 1);
    updateModY(yDest + y0);
    updateModY(yDest + y1 - 1);
  }

  if (y0 > 0) {
    blitImageClipped(src, srcAlpha, 0, 0, xDest, yDest, w, y0);
  }
  if (y1 < h) {
    blitImageClipped(src, srcAlpha, 0, y1, xDest, yDest + y1, w, h - y1);
  }
  if (x0 > 0 && y0 < y1) {
    blitImageClipped(src, srcAlpha, 0, y0, xDest, yDest + y0, x0, y1 - y0);
  }
  if (x1 < w && y0 < y1) {
    blitImageClipped(src, srcAlpha, x1, y0, xDest + x1, yDest + y0,
		     w - x1, y1 - y0);
  }
}
