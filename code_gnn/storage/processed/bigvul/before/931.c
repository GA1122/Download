void CairoOutputDev::updateFont(GfxState *state) {
  cairo_font_face_t *font_face;
  cairo_matrix_t matrix, invert_matrix;

  LOG(printf ("updateFont() font=%s\n", state->getFont()->getName()->getCString()));

  needFontUpdate = gFalse;

  if (text)
    text->updateFont(state);
  
  currentFont = fontEngine->getFont (state->getFont(), xref, catalog, printing);

  if (!currentFont)
    return;

  LOG(printf ("font matrix: %f %f %f %f\n", m11, m12, m21, m22));
  
  font_face = currentFont->getFontFace();
  cairo_set_font_face (cairo, font_face);
 
  double fontSize = state->getFontSize();
  double *m = state->getTextMat();
   
  double w = currentFont->getSubstitutionCorrection(state->getFont());
  matrix.xx = m[0] * fontSize * state->getHorizScaling() * w;
  matrix.yx = m[1] * fontSize * state->getHorizScaling() * w;
  matrix.xy = -m[2] * fontSize;
  matrix.yy = -m[3] * fontSize;
  matrix.x0 = 0;
  matrix.y0 = 0;

  
  invert_matrix = matrix;
  if (cairo_matrix_invert(&invert_matrix)) {
    warning("font matrix not invertible\n");
    return;
  }

  cairo_set_font_matrix (cairo, &matrix);
}
