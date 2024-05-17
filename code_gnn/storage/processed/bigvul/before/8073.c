void Gfx::doAxialShFill(GfxAxialShading *shading) {
  double xMin, yMin, xMax, yMax;
  double x0, y0, x1, y1;
  double dx, dy, mul;
  GBool dxZero, dyZero;
  double bboxIntersections[4];
  double tMin, tMax, tx, ty;
  double s[4], sMin, sMax, tmp;
  double ux0, uy0, ux1, uy1, vx0, vy0, vx1, vy1;
  double t0, t1, tt;
  double ta[axialMaxSplits + 1];
  int next[axialMaxSplits + 1];
  GfxColor color0, color1;
  int nComps;
  int i, j, k;
  GBool needExtend = gTrue;

  state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);

  shading->getCoords(&x0, &y0, &x1, &y1);
  dx = x1 - x0;
  dy = y1 - y0;
  dxZero = fabs(dx) < 0.01;
  dyZero = fabs(dy) < 0.01;
  if (dxZero && dyZero) {
    tMin = tMax = 0;
  } else {
    mul = 1 / (dx * dx + dy * dy);
    bboxIntersections[0] = ((xMin - x0) * dx + (yMin - y0) * dy) * mul;
    bboxIntersections[1] = ((xMin - x0) * dx + (yMax - y0) * dy) * mul;
    bboxIntersections[2] = ((xMax - x0) * dx + (yMin - y0) * dy) * mul;
    bboxIntersections[3] = ((xMax - x0) * dx + (yMax - y0) * dy) * mul;
    bubbleSort(bboxIntersections);
    tMin = bboxIntersections[0];
    tMax = bboxIntersections[3];
    if (tMin < 0 && !shading->getExtend0()) {
      tMin = 0;
    }
    if (tMax > 1 && !shading->getExtend1()) {
      tMax = 1;
    }
  }

  if (out->useShadedFills() &&
      out->axialShadedFill(state, shading, tMin, tMax)) {
	  return;
  }

  t0 = shading->getDomain0();
  t1 = shading->getDomain1();


  nComps = shading->getColorSpace()->getNComps();
  ta[0] = tMin;
  next[0] = axialMaxSplits / 2;
  ta[axialMaxSplits / 2] = 0.5 * (tMin + tMax);
  next[axialMaxSplits / 2] = axialMaxSplits;
  ta[axialMaxSplits] = tMax;

  if (tMin < 0) {
    tt = t0;
  } else if (tMin > 1) {
    tt = t1;
  } else {
    tt = t0 + (t1 - t0) * tMin;
  }
  shading->getColor(tt, &color0);

  if (out->useFillColorStop()) {
    state->setFillColor(&color0);
    out->updateFillColorStop(state, 0);
  }

  tx = x0 + tMin * dx;
  ty = y0 + tMin * dy;
  if (dxZero && dyZero) {
    sMin = sMax = 0;
  } else if (dxZero) {
    sMin = (xMin - tx) / -dy;
    sMax = (xMax - tx) / -dy;
    if (sMin > sMax) { tmp = sMin; sMin = sMax; sMax = tmp; }
  } else if (dyZero) {
    sMin = (yMin - ty) / dx;
    sMax = (yMax - ty) / dx;
    if (sMin > sMax) { tmp = sMin; sMin = sMax; sMax = tmp; }
  } else {
    s[0] = (yMin - ty) / dx;
    s[1] = (yMax - ty) / dx;
    s[2] = (xMin - tx) / -dy;
    s[3] = (xMax - tx) / -dy;
    bubbleSort(s);
    sMin = s[1];
    sMax = s[2];
  }
  ux0 = tx - sMin * dy;
  uy0 = ty + sMin * dx;
  vx0 = tx - sMax * dy;
  vy0 = ty + sMax * dx;

  i = 0;
  bool doneBBox1, doneBBox2;
  if (dxZero && dyZero) {
    doneBBox1 = doneBBox2 = true;
  } else {
    doneBBox1 = bboxIntersections[1] < tMin;
    doneBBox2 = bboxIntersections[2] > tMax;
  }

  needExtend = !out->axialShadedSupportExtend(state, shading);

  while (i < axialMaxSplits) {

    j = next[i];
    while (j > i + 1) {
      if (ta[j] < 0) {
	tt = t0;
      } else if (ta[j] > 1) {
	tt = t1;
      } else {
	tt = t0 + (t1 - t0) * ta[j];
      }
      shading->getColor(tt, &color1);
      if (isSameGfxColor(color1, color0, nComps, axialColorDelta)) {
         if (!doneBBox1 && ta[i] < bboxIntersections[1] && ta[j] > bboxIntersections[1]) {
           int teoricalj = (int) ((bboxIntersections[1] - tMin) * axialMaxSplits / (tMax - tMin));
           if (teoricalj <= i) teoricalj = i + 1;
           if (teoricalj < j) {
             next[i] = teoricalj;
             next[teoricalj] = j;
           }
           else {
             teoricalj = j;
           }
           ta[teoricalj] = bboxIntersections[1];
           j = teoricalj;
           doneBBox1 = true;
         }
         if (!doneBBox2 && ta[i] < bboxIntersections[2] && ta[j] > bboxIntersections[2]) {
           int teoricalj = (int) ((bboxIntersections[2] - tMin) * axialMaxSplits / (tMax - tMin));
           if (teoricalj <= i) teoricalj = i + 1;
           if (teoricalj < j) {
             next[i] = teoricalj;
             next[teoricalj] = j;
           }
           else {
             teoricalj = j;
           }
           ta[teoricalj] = bboxIntersections[2];
           j = teoricalj;
           doneBBox2 = true;
         }
         break;
      }
      k = (i + j) / 2;
      ta[k] = 0.5 * (ta[i] + ta[j]);
      next[i] = k;
      next[k] = j;
      j = k;
    }

    for (k = 0; k < nComps; ++k) {
      color0.c[k] = (color0.c[k] + color1.c[k]) / 2;
    }

    tx = x0 + ta[j] * dx;
    ty = y0 + ta[j] * dy;
    if (dxZero && dyZero) {
      sMin = sMax = 0;
    } else if (dxZero) {
      sMin = (xMin - tx) / -dy;
      sMax = (xMax - tx) / -dy;
      if (sMin > sMax) { tmp = sMin; sMin = sMax; sMax = tmp; }
    } else if (dyZero) {
      sMin = (yMin - ty) / dx;
      sMax = (yMax - ty) / dx;
      if (sMin > sMax) { tmp = sMin; sMin = sMax; sMax = tmp; }
    } else {
      s[0] = (yMin - ty) / dx;
      s[1] = (yMax - ty) / dx;
      s[2] = (xMin - tx) / -dy;
      s[3] = (xMax - tx) / -dy;
      bubbleSort(s);
      sMin = s[1];
      sMax = s[2];
    }
    ux1 = tx - sMin * dy;
    uy1 = ty + sMin * dx;
    vx1 = tx - sMax * dy;
    vy1 = ty + sMax * dx;

    state->setFillColor(&color0);
    if (out->useFillColorStop())
      out->updateFillColorStop(state, (ta[j] - tMin)/(tMax - tMin));
    else
      out->updateFillColor(state);

    if (needExtend) {
      state->moveTo(ux0, uy0);
      state->lineTo(vx0, vy0);
      state->lineTo(vx1, vy1);
      state->lineTo(ux1, uy1);
      state->closePath();
    }

    if (!out->useFillColorStop()) {
      if (!contentIsHidden())
        out->fill(state);
      state->clearPath();
    }

    ux0 = ux1;
    uy0 = uy1;
    vx0 = vx1;
    vy0 = vy1;
    color0 = color1;
    i = next[i];
  }

  if (out->useFillColorStop()) {
    if (!needExtend) {
      state->moveTo(xMin, yMin);
      state->lineTo(xMin, yMax);
      state->lineTo(xMax, yMax);
      state->lineTo(xMax, yMin);
      state->closePath();
    }
    if (!contentIsHidden())
      out->fill(state);
    state->clearPath();
  }
}
