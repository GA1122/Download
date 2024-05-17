void CairoOutputDev::paintTransparencyGroup(GfxState *  , double *  ) {
  cairo_set_source (cairo, group);

  if (!mask) {
    if (shape) {
      cairo_save (cairo);

       

       
      cairo_set_source (cairo, shape);
      cairo_set_operator (cairo, CAIRO_OPERATOR_DEST_OUT);
      cairo_paint (cairo);

      cairo_set_operator (cairo, CAIRO_OPERATOR_ADD);
      cairo_set_source (cairo, group);
      cairo_paint (cairo);

      cairo_restore (cairo);

      cairo_pattern_destroy (shape);
      shape = NULL;
    } else {
      cairo_paint_with_alpha (cairo, fill_opacity);
    }
    cairo_status_t status = cairo_status(cairo);
    if (status)
      printf("BAD status: %s\n", cairo_status_to_string(status));
  } else {
    cairo_mask(cairo, mask);

    cairo_pattern_destroy(mask);
    mask = NULL;
  }

  popTransparencyGroup();
}
