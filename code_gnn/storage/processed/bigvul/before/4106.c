GBool Splash::gouraudTriangleShadedFill(SplashGouraudColor *shading)
{
  double xdbl[3] = {0., 0., 0.};
  double ydbl[3] = {0., 0., 0.};
  int    x[3] = {0, 0, 0};
  int    y[3] = {0, 0, 0};
  double xt=0., xa=0., yt=0.;
  double ca=0., ct=0.;

  double scanLimitMapL[2] = {0., 0.};
  double scanLimitMapR[2] = {0., 0.};
  double scanColorMapL[2] = {0., 0.};
  double scanColorMapR[2] = {0., 0.};
  double scanColorMap[2] = {0., 0.};
  int scanEdgeL[2] = { 0, 0 };
  int scanEdgeR[2] = { 0, 0 };
  GBool hasFurtherSegment = gFalse;

  int scanLineOff = 0;
  int bitmapOff = 0;
  int scanLimitR = 0, scanLimitL = 0;

  int bitmapWidth = bitmap->getWidth();
  SplashClip* clip = getClip();
  SplashBitmap *blitTarget = bitmap;
  SplashColorPtr bitmapData = bitmap->getDataPtr();
  SplashColorPtr bitmapAlpha = bitmap->getAlphaPtr();
  SplashColorPtr cur = NULL;
  SplashCoord* userToCanvasMatrix = getMatrix();
  SplashColorMode bitmapMode = bitmap->getMode();
  GBool hasAlpha = (bitmapAlpha != NULL);
  int rowSize = bitmap->getRowSize();
  int colorComps = 0;
  switch (bitmapMode) {
    case splashModeMono1:
    break;
    case splashModeMono8:
      colorComps=1;
    break;
    case splashModeRGB8:
      colorComps=3;
    break;
    case splashModeBGR8:
      colorComps=3;
    break;
    case splashModeXBGR8:
      colorComps=4;
    break;
#if SPLASH_CMYK
    case splashModeCMYK8:
      colorComps=4;
    break;
    case splashModeDeviceN8:
      colorComps=SPOT_NCOMPS+4;
    break;
#endif
  }

  SplashPipe pipe;
  SplashColor cSrcVal;

  pipeInit(&pipe, 0, 0, NULL, cSrcVal, (Guchar)splashRound(state->strokeAlpha * 255), gFalse, gFalse);

  if (vectorAntialias) {
    if (aaBuf == NULL)
      return gFalse;  
    drawAAPixelInit();
  }

  GBool bDirectBlit = vectorAntialias ? gFalse : pipe.noTransparency && !state->blendFunc;
  if (!bDirectBlit) {
    blitTarget = new SplashBitmap(bitmap->getWidth(),
                                  bitmap->getHeight(),
                                  bitmap->getRowPad(),
                                  bitmap->getMode(),
                                  gTrue,
                                  bitmap->getRowSize() >= 0);
    bitmapData = blitTarget->getDataPtr();
    bitmapAlpha = blitTarget->getAlphaPtr();

    int S = bitmap->getWidth() * bitmap->getHeight();
    for (int i = 0; i < S; ++i)
      bitmapAlpha[i] = 0;
    hasAlpha = gTrue;
  }

  if (shading->isParameterized()) {
    double color[3];
    double colorinterp;

    for (int i = 0; i < shading->getNTriangles(); ++i) {
      shading->getTriangle(i,
                           xdbl + 0, ydbl + 0, color + 0,
                           xdbl + 1, ydbl + 1, color + 1,
                           xdbl + 2, ydbl + 2, color + 2);
      for (int m = 0; m < 3; ++m) {
        xt = xdbl[m] * (double)userToCanvasMatrix[0] + ydbl[m] * (double)userToCanvasMatrix[2] + (double)userToCanvasMatrix[4];
        yt = xdbl[m] * (double)userToCanvasMatrix[1] + ydbl[m] * (double)userToCanvasMatrix[3] + (double)userToCanvasMatrix[5];
        xdbl[m] = xt;
        ydbl[m] = yt;
        x[m] = splashRound(xt);
        y[m] = splashRound(yt);
      }
      if (y[0] > y[1]) {
        Guswap(x[0], x[1]);
        Guswap(y[0], y[1]);
        Guswap(color[0], color[1]);
      }
      assert(y[0] <= y[1]);
      if (y[1] > y[2]) {
        int tmpX = x[2];
        int tmpY = y[2];
        double tmpC = color[2];
        x[2] = x[1]; y[2] = y[1]; color[2] = color[1];

        if (y[0] > tmpY) {
          x[1] = x[0]; y[1] = y[0]; color[1] = color[0];
          x[0] = tmpX; y[0] = tmpY; color[0] = tmpC;
        } else {
          x[1] = tmpX; y[1] = tmpY; color[1] = tmpC;
        }
      }
      assert(y[0] <= y[1]);
      assert(y[1] <= y[2]);

      if ((x[0] - x[2]) * (y[1] - y[2]) - (x[1] - x[2]) * (y[0] - y[2]) == 0)
        continue;  

      scanEdgeL[0] = 0;
      scanEdgeR[0] = 0;
      if (y[0] == y[1]) {
        scanEdgeL[0] = 1;
        scanEdgeL[1] = scanEdgeR[1] = 2;

      } else {
        scanEdgeL[1] = 1; scanEdgeR[1] = 2;
      }
      assert(y[scanEdgeL[0]] < y[scanEdgeL[1]]);
      assert(y[scanEdgeR[0]] < y[scanEdgeR[1]]);

      scanLimitMapL[0] = double(x[scanEdgeL[1]] - x[scanEdgeL[0]]) / (y[scanEdgeL[1]] - y[scanEdgeL[0]]);
      scanLimitMapL[1] = x[scanEdgeL[0]] - y[scanEdgeL[0]] * scanLimitMapL[0];
      scanLimitMapR[0] = double(x[scanEdgeR[1]] - x[scanEdgeR[0]]) / (y[scanEdgeR[1]] - y[scanEdgeR[0]]);
      scanLimitMapR[1] = x[scanEdgeR[0]] - y[scanEdgeR[0]] * scanLimitMapR[0];

      xa = y[1] * scanLimitMapL[0] + scanLimitMapL[1];
      xt = y[1] * scanLimitMapR[0] + scanLimitMapR[1];
      if (xa > xt) {
        Guswap(scanEdgeL[0], scanEdgeR[0]);
        Guswap(scanEdgeL[1], scanEdgeR[1]);
        Guswap(scanLimitMapL[0], scanLimitMapR[0]);
        Guswap(scanLimitMapL[1], scanLimitMapR[1]);
      }

      scanColorMapL[0] = (color[scanEdgeL[1]] - color[scanEdgeL[0]]) / (y[scanEdgeL[1]] - y[scanEdgeL[0]]);
      scanColorMapL[1] = color[scanEdgeL[0]] - y[scanEdgeL[0]] * scanColorMapL[0];
      scanColorMapR[0] = (color[scanEdgeR[1]] - color[scanEdgeR[0]]) / (y[scanEdgeR[1]] - y[scanEdgeR[0]]);
      scanColorMapR[1] = color[scanEdgeR[0]] - y[scanEdgeR[0]] * scanColorMapR[0];

      hasFurtherSegment = (y[1] < y[2]);
      scanLineOff = y[0] * rowSize;

      for (int Y = y[0]; Y <= y[2]; ++Y, scanLineOff += rowSize) {
        if (hasFurtherSegment && Y == y[1]) {
          if (scanEdgeL[1] == 1) {
            scanEdgeL[0] = 1;
            scanEdgeL[1] = 2;
            scanLimitMapL[0] = double(x[scanEdgeL[1]] - x[scanEdgeL[0]]) / (y[scanEdgeL[1]] - y[scanEdgeL[0]]);
            scanLimitMapL[1] = x[scanEdgeL[0]] - y[scanEdgeL[0]] * scanLimitMapL[0];

            scanColorMapL[0] = (color[scanEdgeL[1]] - color[scanEdgeL[0]]) / (y[scanEdgeL[1]] - y[scanEdgeL[0]]);
            scanColorMapL[1] = color[scanEdgeL[0]] - y[scanEdgeL[0]] * scanColorMapL[0];
          } else if (scanEdgeR[1] == 1) {
            scanEdgeR[0] = 1;
            scanEdgeR[1] = 2;
            scanLimitMapR[0] = double(x[scanEdgeR[1]] - x[scanEdgeR[0]]) / (y[scanEdgeR[1]] - y[scanEdgeR[0]]);
            scanLimitMapR[1] = x[scanEdgeR[0]] - y[scanEdgeR[0]] * scanLimitMapR[0];

            scanColorMapR[0] = (color[scanEdgeR[1]] - color[scanEdgeR[0]]) / (y[scanEdgeR[1]] - y[scanEdgeR[0]]);
            scanColorMapR[1] = color[scanEdgeR[0]] - y[scanEdgeR[0]] * scanColorMapR[0];
          }
          assert( y[scanEdgeL[0]]  <  y[scanEdgeL[1]] );
          assert( y[scanEdgeR[0]] <  y[scanEdgeR[1]] );
          hasFurtherSegment = gFalse;
        }

        yt = Y;

        xa = yt * scanLimitMapL[0] + scanLimitMapL[1];
        xt = yt * scanLimitMapR[0] + scanLimitMapR[1];

        ca = yt * scanColorMapL[0] + scanColorMapL[1];
        ct = yt * scanColorMapR[0] + scanColorMapR[1];

        scanLimitL = splashRound(xa);
        scanLimitR = splashRound(xt);

        scanColorMap[0] = (scanLimitR == scanLimitL) ? 0. : ((ct - ca) / (scanLimitR - scanLimitL));
        scanColorMap[1] = ca - scanLimitL * scanColorMap[0];

        assert(scanLimitL <= scanLimitR || abs(scanLimitL - scanLimitR) <= 2);  
        assert(scanLineOff == Y * rowSize);

        colorinterp = scanColorMap[0] * scanLimitL + scanColorMap[1];

        bitmapOff = scanLineOff + scanLimitL * colorComps;
        for (int X = scanLimitL; X <= scanLimitR; ++X, colorinterp += scanColorMap[0], bitmapOff += colorComps) {
          if (!clip->test(X, Y))
            continue;

          assert(fabs(colorinterp - (scanColorMap[0] * X + scanColorMap[1])) < 1e-10);
          assert(bitmapOff == Y * rowSize + colorComps * X && scanLineOff == Y * rowSize);

          shading->getParameterizedColor(colorinterp, bitmapMode, &bitmapData[bitmapOff]);

          if (hasAlpha)
            bitmapAlpha[Y * bitmapWidth + X] = 255;
        }
      }
    }
  } else {
    return gFalse;
  }

  if (!bDirectBlit) {
    int W = blitTarget->getWidth();
    int H = blitTarget->getHeight();
    cur = cSrcVal;

    for (int X = 0; X < W; ++X) {
      for (int Y = 0; Y < H; ++Y) {
        if (!bitmapAlpha[Y * bitmapWidth + X])
          continue;  
        bitmapOff = Y * rowSize + colorComps * X;

        for (int m = 0; m < colorComps; ++m)
          cur[m] = bitmapData[bitmapOff + m];
        if (vectorAntialias) {
          drawAAPixel(&pipe, X, Y);
        } else {
          drawPixel(&pipe, X, Y, gTrue);  
        }
      }
    }

    delete blitTarget;
    blitTarget = NULL;
  }

  return gTrue;
}
