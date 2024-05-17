void Gfx::doRadialShFill(GfxRadialShading *shading) {
  double xMin, yMin, xMax, yMax;
  double x0, y0, r0, x1, y1, r1, t0, t1;
  int nComps;
  GfxColor colorA, colorB;
  double xa, ya, xb, yb, ra, rb;
  double ta, tb, sa, sb;
  double sz, xz, yz, sMin, sMax;
  GBool enclosed;
  int ia, ib, k, n;
  double *ctm;
  double theta, alpha, angle, t;
  GBool needExtend = gTrue;

  shading->getCoords(&x0, &y0, &r0, &x1, &y1, &r1);
  t0 = shading->getDomain0();
  t1 = shading->getDomain1();
  nComps = shading->getColorSpace()->getNComps();

  if (x0 == x1 && y0 == y1) {
    enclosed = gTrue;
    theta = 0;  
    sz = 0;  
  } else if (r0 == r1) {
    enclosed = gFalse;
    theta = 0;
    sz = 0;  
  } else {
    sz = -r0 / (r1 - r0);
    xz = x0 + sz * (x1 - x0);
    yz = y0 + sz * (y1 - y0);
    enclosed = (xz - x0) * (xz - x0) + (yz - y0) * (yz - y0) <= r0 * r0;
    theta = asin(r0 / sqrt((x0 - xz) * (x0 - xz) + (y0 - yz) * (y0 - yz)));
    if (r0 > r1) {
      theta = -theta;
    }
  }
  if (enclosed) {
    alpha = 0;
  } else {
    alpha = atan2(y1 - y0, x1 - x0);
  }

  state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);
  if (enclosed) {
    sMin = 0;
    sMax = 1;
  } else {
    sMin = 1;
    sMax = 0;
    if ((x1 + r1) - (x0 + r0) != 0) {
      sa = (xMin - (x0 + r0)) / ((x1 + r1) - (x0 + r0));
      if (sa < sMin) {
	sMin = sa;
      } else if (sa > sMax) {
	sMax = sa;
      }
    }
    if ((x1 - r1) - (x0 - r0) != 0) {
      sa = (xMax - (x0 - r0)) / ((x1 - r1) - (x0 - r0));
      if (sa < sMin) {
	sMin = sa;
      } else if (sa > sMax) {
	sMax = sa;
      }
    }
    if ((y1 + r1) - (y0 + r0) != 0) {
      sa = (yMin - (y0 + r0)) / ((y1 + r1) - (y0 + r0));
      if (sa < sMin) {
	sMin = sa;
      } else if (sa > sMax) {
	sMax = sa;
      }
    }
    if ((y1 - r1) - (y0 - r0) != 0) {
      sa = (yMax - (y0 - r0)) / ((y1 - r1) - (y0 - r0));
      if (sa < sMin) {
	sMin = sa;
      } else if (sa > sMax) {
	sMax = sa;
      }
    }
    if (r0 < r1) {
      if (sMin < sz) {
	sMin = sz;
      }
    } else if (r0 > r1) {
      if (sMax > sz) {
	sMax = sz;
      }
    }
    if (!shading->getExtend0() && sMin < 0) {
      sMin = 0;
    }
    if (!shading->getExtend1() && sMax > 1) {
      sMax = 1;
    }
  }

  if (out->useShadedFills() &&
      out->radialShadedFill(state, shading, sMin, sMax)) {
    return;
  }

  ctm = state->getCTM();
  t = fabs(ctm[0]);
  if (fabs(ctm[1]) > t) {
    t = fabs(ctm[1]);
  }
  if (fabs(ctm[2]) > t) {
    t = fabs(ctm[2]);
  }
  if (fabs(ctm[3]) > t) {
    t = fabs(ctm[3]);
  }
  if (r0 > r1) {
    t *= r0;
  } else {
    t *= r1;
  }
  if (t < 1) {
    n = 3;
  } else {
    n = (int)(M_PI / acos(1 - 0.1 / t));
    if (n < 3) {
      n = 3;
    } else if (n > 200) {
      n = 200;
    }
  }

  ia = 0;
  sa = sMin;
  ta = t0 + sa * (t1 - t0);
  xa = x0 + sa * (x1 - x0);
  ya = y0 + sa * (y1 - y0);
  ra = r0 + sa * (r1 - r0);
  if (ta < t0) {
    shading->getColor(t0, &colorA);
  } else if (ta > t1) {
    shading->getColor(t1, &colorA);
  } else {
    shading->getColor(ta, &colorA);
  }

  needExtend = !out->radialShadedSupportExtend(state, shading);

  while (ia < radialMaxSplits) {

    ib = radialMaxSplits;
    sb = sMax;
    tb = t0 + sb * (t1 - t0);
    if (tb < t0) {
      shading->getColor(t0, &colorB);
    } else if (tb > t1) {
      shading->getColor(t1, &colorB);
    } else {
      shading->getColor(tb, &colorB);
    }
    while (ib - ia > 1) {
      if (isSameGfxColor(colorB, colorA, nComps, radialColorDelta) && ib < radialMaxSplits) {
	GfxColor colorC;
	int ic = (ia + ib) / 2;
	double sc = sMin + ((double)ic / (double)radialMaxSplits) * (sMax - sMin);
	double tc = t0 + sc * (t1 - t0);
	if (tc < t0) {
	  shading->getColor(t0, &colorC);
	} else if (tc > t1) {
	  shading->getColor(t1, &colorC);
	} else {
	  shading->getColor(tc, &colorC);
	}
	if (isSameGfxColor(colorC, colorA, nComps, radialColorDelta))
	  break;
      }
      ib = (ia + ib) / 2;
      sb = sMin + ((double)ib / (double)radialMaxSplits) * (sMax - sMin);
      tb = t0 + sb * (t1 - t0);
      if (tb < t0) {
	shading->getColor(t0, &colorB);
      } else if (tb > t1) {
	shading->getColor(t1, &colorB);
      } else {
	shading->getColor(tb, &colorB);
      }
    }

    xb = x0 + sb * (x1 - x0);
    yb = y0 + sb * (y1 - y0);
    rb = r0 + sb * (r1 - r0);

    for (k = 0; k < nComps; ++k) {
      colorA.c[k] = (colorA.c[k] + colorB.c[k]) / 2;
    }
    state->setFillColor(&colorA);
    if (out->useFillColorStop())
      out->updateFillColorStop(state, (sa - sMin)/(sMax - sMin));
    else
      out->updateFillColor(state);

    if (needExtend) {
      if (enclosed) {
        state->moveTo(xa + ra, ya);
        for (k = 1; k < n; ++k) {
	  angle = ((double)k / (double)n) * 2 * M_PI;
	  state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
        }
        state->closePath();

        state->moveTo(xb + rb, yb);
        for (k = 1; k < n; ++k) {
	  angle = -((double)k / (double)n) * 2 * M_PI;
	  state->lineTo(xb + rb * cos(angle), yb + rb * sin(angle));
        }
        state->closePath();
      } else {
        state->moveTo(xa + ra * cos(alpha + theta + 0.5 * M_PI),
		      ya + ra * sin(alpha + theta + 0.5 * M_PI));
        for (k = 0; k < n; ++k) {
	  angle = alpha + theta + 0.5 * M_PI
		  - ((double)k / (double)n) * (2 * theta + M_PI);
	  state->lineTo(xb + rb * cos(angle), yb + rb * sin(angle));
        }
	for (k = 0; k < n; ++k) {
	  angle = alpha - theta - 0.5 * M_PI
		  + ((double)k / (double)n) * (2 * theta - M_PI);
	  state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
	}
	state->closePath();

	state->moveTo(xa + ra * cos(alpha + theta + 0.5 * M_PI),
		      ya + ra * sin(alpha + theta + 0.5 * M_PI));
	for (k = 0; k < n; ++k) {
	  angle = alpha + theta + 0.5 * M_PI
		  + ((double)k / (double)n) * (-2 * theta + M_PI);
	  state->lineTo(xb + rb * cos(angle), yb + rb * sin(angle));
	}
	for (k = 0; k < n; ++k) {
	  angle = alpha - theta - 0.5 * M_PI
		  + ((double)k / (double)n) * (2 * theta + M_PI);
	  state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
	}
	state->closePath();
      }
    }

    if (!out->useFillColorStop()) {
      if (!contentIsHidden())
        out->fill(state);
      state->clearPath();
    }

    ia = ib;
    sa = sb;
    ta = tb;
    xa = xb;
    ya = yb;
    ra = rb;
    colorA = colorB;
  }

  if (out->useFillColorStop()) {
    state->setFillColor(&colorA);
    out->updateFillColorStop(state, (sb - sMin)/(sMax - sMin));

    state->moveTo(xMin, yMin);
    state->lineTo(xMin, yMax);
    state->lineTo(xMax, yMax);
    state->lineTo(xMax, yMin);
    state->closePath();

    if (!contentIsHidden())
      out->fill(state);
    state->clearPath();
  }

  if (!needExtend)
    return;

  if (enclosed) {
    if ((shading->getExtend0() && r0 <= r1) ||
	(shading->getExtend1() && r1 < r0)) {
      if (r0 <= r1) {
	ta = t0;
	ra = r0;
	xa = x0;
	ya = y0;
      } else {
	ta = t1;
	ra = r1;
	xa = x1;
	ya = y1;
      }
      shading->getColor(ta, &colorA);
      state->setFillColor(&colorA);
      out->updateFillColor(state);
      state->moveTo(xa + ra, ya);
      for (k = 1; k < n; ++k) {
	angle = ((double)k / (double)n) * 2 * M_PI;
	state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
      }
      state->closePath();
      if (!contentIsHidden())
        out->fill(state);
      state->clearPath();
    }

    if ((shading->getExtend0() && r0 > r1) ||
	(shading->getExtend1() && r1 >= r0)) {
      if (r0 > r1) {
	ta = t0;
	ra = r0;
	xa = x0;
	ya = y0;
      } else {
	ta = t1;
	ra = r1;
	xa = x1;
	ya = y1;
      }
      shading->getColor(ta, &colorA);
      state->setFillColor(&colorA);
      out->updateFillColor(state);
      state->moveTo(xMin, yMin);
      state->lineTo(xMin, yMax);
      state->lineTo(xMax, yMax);
      state->lineTo(xMax, yMin);
      state->closePath();
      state->moveTo(xa + ra, ya);
      for (k = 1; k < n; ++k) {
	angle = ((double)k / (double)n) * 2 * M_PI;
	state->lineTo(xa + ra * cos(angle), ya + ra * sin(angle));
      }
      state->closePath();
      if (!contentIsHidden())
        out->fill(state);
      state->clearPath();
    }
  }
}
