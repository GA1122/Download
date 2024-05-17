void Splash::scaleImageYuXu(SplashImageSource src, void *srcData,
			    SplashColorMode srcMode, int nComps,
			    GBool srcAlpha, int srcWidth, int srcHeight,
			    int scaledWidth, int scaledHeight,
			    SplashBitmap *dest) {
  Guchar *lineBuf, *alphaLineBuf;
  Guint pix[splashMaxColorComps];
  Guint alpha;
  Guchar *destPtr0, *destPtr, *destAlphaPtr0, *destAlphaPtr;
  int yp, yq, xp, xq, yt, y, yStep, xt, x, xStep, xx;
  int i, j;

  yp = scaledHeight / srcHeight;
  yq = scaledHeight % srcHeight;

  xp = scaledWidth / srcWidth;
  xq = scaledWidth % srcWidth;

  lineBuf = (Guchar *)gmallocn(srcWidth, nComps);
  if (srcAlpha) {
    alphaLineBuf = (Guchar *)gmalloc(srcWidth);
  } else {
    alphaLineBuf = NULL;
  }

  yt = 0;

  destPtr0 = dest->data;
  destAlphaPtr0 = dest->alpha;
  for (y = 0; y < srcHeight; ++y) {

    if ((yt += yq) >= srcHeight) {
      yt -= srcHeight;
      yStep = yp + 1;
    } else {
      yStep = yp;
    }

    (*src)(srcData, lineBuf, alphaLineBuf);

    xt = 0;

    xx = 0;
    for (x = 0; x < srcWidth; ++x) {

      if ((xt += xq) >= srcWidth) {
	xt -= srcWidth;
	xStep = xp + 1;
      } else {
	xStep = xp;
      }

      for (i = 0; i < nComps; ++i) {
	pix[i] = lineBuf[x * nComps + i];
      }

      switch (srcMode) {
      case splashModeMono1:  
	break;
      case splashModeMono8:
	for (i = 0; i < yStep; ++i) {
	  for (j = 0; j < xStep; ++j) {
	    destPtr = destPtr0 + (i * scaledWidth + xx + j) * nComps;
	    *destPtr++ = (Guchar)pix[0];
	  }
	}
	break;
      case splashModeRGB8:
	for (i = 0; i < yStep; ++i) {
	  for (j = 0; j < xStep; ++j) {
	    destPtr = destPtr0 + (i * scaledWidth + xx + j) * nComps;
	    *destPtr++ = (Guchar)pix[0];
	    *destPtr++ = (Guchar)pix[1];
	    *destPtr++ = (Guchar)pix[2];
	  }
	}
	break;
      case splashModeXBGR8:
	for (i = 0; i < yStep; ++i) {
	  for (j = 0; j < xStep; ++j) {
	    destPtr = destPtr0 + (i * scaledWidth + xx + j) * nComps;
	    *destPtr++ = (Guchar)pix[2];
	    *destPtr++ = (Guchar)pix[1];
	    *destPtr++ = (Guchar)pix[0];
	    *destPtr++ = (Guchar)255;
	  }
	}
	break;
      case splashModeBGR8:
	for (i = 0; i < yStep; ++i) {
	  for (j = 0; j < xStep; ++j) {
	    destPtr = destPtr0 + (i * scaledWidth + xx + j) * nComps;
	    *destPtr++ = (Guchar)pix[2];
	    *destPtr++ = (Guchar)pix[1];
	    *destPtr++ = (Guchar)pix[0];
	  }
	}
	break;
#if SPLASH_CMYK
      case splashModeCMYK8:
	for (i = 0; i < yStep; ++i) {
	  for (j = 0; j < xStep; ++j) {
	    destPtr = destPtr0 + (i * scaledWidth + xx + j) * nComps;
	    *destPtr++ = (Guchar)pix[0];
	    *destPtr++ = (Guchar)pix[1];
	    *destPtr++ = (Guchar)pix[2];
	    *destPtr++ = (Guchar)pix[3];
	  }
	}
	break;
      case splashModeDeviceN8:
	for (i = 0; i < yStep; ++i) {
	  for (j = 0; j < xStep; ++j) {
	    destPtr = destPtr0 + (i * scaledWidth + xx + j) * nComps;
      for (int cp = 0; cp < SPOT_NCOMPS+4; cp++)
        *destPtr++ = (Guchar)pix[cp];
	  }
	}
	break;
#endif
      }

      if (srcAlpha) {
	alpha = alphaLineBuf[x];
	for (i = 0; i < yStep; ++i) {
	  for (j = 0; j < xStep; ++j) {
	    destAlphaPtr = destAlphaPtr0 + i * scaledWidth + xx + j;
	    *destAlphaPtr = (Guchar)alpha;
	  }
	}
      }

      xx += xStep;
    }

    destPtr0 += yStep * scaledWidth * nComps;
    if (srcAlpha) {
      destAlphaPtr0 += yStep * scaledWidth;
    }
  }

  gfree(alphaLineBuf);
  gfree(lineBuf);
}
