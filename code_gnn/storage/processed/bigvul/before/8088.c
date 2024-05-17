void Gfx::drawAnnot(Object *str, AnnotBorder *border, AnnotColor *aColor,
		    double xMin, double yMin, double xMax, double yMax) {
  Dict *dict, *resDict;
  Object matrixObj, bboxObj, resObj;
  Object obj1;
  double m[6], bbox[4], ictm[6];
  double *ctm;
  double formX0, formY0, formX1, formY1;
  double annotX0, annotY0, annotX1, annotY1;
  double det, x, y, sx, sy;
  double r, g, b;
  GfxColor color;
  double *dash, *dash2;
  int dashLength;
  int i;


  ctm = state->getCTM();
  det = 1 / (ctm[0] * ctm[3] - ctm[1] * ctm[2]);
  ictm[0] = ctm[3] * det;
  ictm[1] = -ctm[1] * det;
  ictm[2] = -ctm[2] * det;
  ictm[3] = ctm[0] * det;
  ictm[4] = (ctm[2] * ctm[5] - ctm[3] * ctm[4]) * det;
  ictm[5] = (ctm[1] * ctm[4] - ctm[0] * ctm[5]) * det;
  x = baseMatrix[0] * xMin + baseMatrix[2] * yMin + baseMatrix[4];
  y = baseMatrix[1] * xMin + baseMatrix[3] * yMin + baseMatrix[5];
  annotX0 = ictm[0] * x + ictm[2] * y + ictm[4];
  annotY0 = ictm[1] * x + ictm[3] * y + ictm[5];
  x = baseMatrix[0] * xMax + baseMatrix[2] * yMax + baseMatrix[4];
  y = baseMatrix[1] * xMax + baseMatrix[3] * yMax + baseMatrix[5];
  annotX1 = ictm[0] * x + ictm[2] * y + ictm[4];
  annotY1 = ictm[1] * x + ictm[3] * y + ictm[5];
  if (annotX0 > annotX1) {
    x = annotX0; annotX0 = annotX1; annotX1 = x;
  }
  if (annotY0 > annotY1) {
    y = annotY0; annotY0 = annotY1; annotY1 = y;
  }

  if (str->isStream()) {

    dict = str->streamGetDict();

    dict->lookup("BBox", &bboxObj);
    if (!bboxObj.isArray()) {
      bboxObj.free();
      error(getPos(), "Bad form bounding box");
      return;
    }
    for (i = 0; i < 4; ++i) {
      bboxObj.arrayGet(i, &obj1);
      bbox[i] = obj1.getNum();
      obj1.free();
    }
    bboxObj.free();

    dict->lookup("Matrix", &matrixObj);
    if (matrixObj.isArray() && matrixObj.arrayGetLength() >= 6) {
      for (i = 0; i < 6; ++i) {
	matrixObj.arrayGet(i, &obj1);
	m[i] = obj1.getNum();
	obj1.free();
      }
    } else {
      m[0] = 1; m[1] = 0;
      m[2] = 0; m[3] = 1;
      m[4] = 0; m[5] = 0;
    }
    matrixObj.free();

    formX0 = bbox[0] * m[0] + bbox[1] * m[2] + m[4];
    formY0 = bbox[0] * m[1] + bbox[1] * m[3] + m[5];
    formX1 = bbox[2] * m[0] + bbox[3] * m[2] + m[4];
    formY1 = bbox[2] * m[1] + bbox[3] * m[3] + m[5];
    if (formX0 > formX1) {
      x = formX0; formX0 = formX1; formX1 = x;
    }
    if (formY0 > formY1) {
      y = formY0; formY0 = formY1; formY1 = y;
    }

    if (formX1 == formX0) {
      sx = 1;
    } else {
      sx = (annotX1 - annotX0) / (formX1 - formX0);
    }
    if (formY1 == formY0) {
      sy = 1;
    } else {
      sy = (annotY1 - annotY0) / (formY1 - formY0);
    }
    m[0] *= sx;
    m[2] *= sx;
    m[4] = (m[4] - formX0) * sx + annotX0;
    m[1] *= sy;
    m[3] *= sy;
    m[5] = (m[5] - formY0) * sy + annotY0;

    dict->lookup("Resources", &resObj);
    resDict = resObj.isDict() ? resObj.getDict() : (Dict *)NULL;

    doForm1(str, resDict, m, bbox);

    resObj.free();
  }

  if (border && border->getWidth() > 0) {
    if (state->getStrokeColorSpace()->getMode() != csDeviceRGB) {
      state->setStrokePattern(NULL);
      state->setStrokeColorSpace(new GfxDeviceRGBColorSpace());
      out->updateStrokeColorSpace(state);
    }
    if (aColor && (aColor->getSpace() == AnnotColor::colorRGB)) {
      const double *values = aColor->getValues();
      r = values[0];
      g = values[1];
      b = values[2];
    } else {
      r = g = b = 0;
    };
    color.c[0] = dblToCol(r);
    color.c[1] = dblToCol(g);
    color.c[2] = dblToCol(b);
    state->setStrokeColor(&color);
    out->updateStrokeColor(state);
    x = (baseMatrix[0] + baseMatrix[2]) * ictm[0] +
        (baseMatrix[1] + baseMatrix[3]) * ictm[2];
    y = (baseMatrix[0] + baseMatrix[2]) * ictm[1] +
        (baseMatrix[1] + baseMatrix[3]) * ictm[3];
    x = sqrt(0.5 * (x * x + y * y));
    state->setLineWidth(x * border->getWidth());
    out->updateLineWidth(state);
    dashLength = border->getDashLength();
    dash = border->getDash();
    if (border->getStyle() == AnnotBorder::borderDashed && dashLength > 0) {
      dash2 = (double *)gmallocn(dashLength, sizeof(double));
      for (i = 0; i < dashLength; ++i) {
	dash2[i] = x * dash[i];
      }
      state->setLineDash(dash2, dashLength, 0);
      out->updateLineDash(state);
    }
    state->clearPath();
    state->moveTo(annotX0, out->upsideDown() ? annotY0 : annotY1);
    state->lineTo(annotX1, out->upsideDown() ? annotY0 : annotY1);
    if (border->getStyle() != AnnotBorder::borderUnderlined) {
      state->lineTo(annotX1, out->upsideDown() ? annotY1 : annotY0);
      state->lineTo(annotX0, out->upsideDown() ? annotY1 : annotY0);
      state->closePath();
    }
    out->stroke(state);
  }
}
