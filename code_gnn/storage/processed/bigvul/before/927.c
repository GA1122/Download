void CairoOutputDev::updateCTM(GfxState *state, double m11, double m12,
				double m21, double m22,
				double m31, double m32) {
  cairo_matrix_t matrix, invert_matrix;
  matrix.xx = m11;
  matrix.yx = m12;
  matrix.xy = m21;
  matrix.yy = m22;
  matrix.x0 = m31;
  matrix.y0 = m32;

   
  invert_matrix = matrix;
  if (cairo_matrix_invert(&invert_matrix)) {
    warning("matrix not invertible\n");
    return;
  }

  cairo_transform (cairo, &matrix);
  if (cairo_shape)
    cairo_transform (cairo_shape, &matrix);
  updateLineDash(state);
  updateLineJoin(state);
  updateLineCap(state);
  updateLineWidth(state);
}