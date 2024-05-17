void CairoOutputDev::eoClip(GfxState *state) {
  doPath (cairo, state, state->getPath());
  cairo_set_fill_rule (cairo, CAIRO_FILL_RULE_EVEN_ODD);
  cairo_clip (cairo);
  LOG (printf ("clip-eo\n"));
  if (cairo_shape) {
    doPath (cairo_shape, state, state->getPath());
    cairo_set_fill_rule (cairo_shape, CAIRO_FILL_RULE_EVEN_ODD);
    cairo_clip (cairo_shape);
  }

}