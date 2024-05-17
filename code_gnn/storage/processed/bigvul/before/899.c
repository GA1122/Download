void CairoOutputDev::endString(GfxState *state)
{
  int render;

  if (!currentFont)
    return;

  if (!glyphs)
    return;

  render = state->getRender();
  if (render == 3 || glyphCount == 0) {
    gfree(glyphs);
    glyphs = NULL;
    return;
  }
  
  if (!(render & 1)) {
    LOG (printf ("fill string\n"));
    cairo_set_source (cairo, fill_pattern);
    cairo_show_glyphs (cairo, glyphs, glyphCount);
    if (cairo_shape)
      cairo_show_glyphs (cairo_shape, glyphs, glyphCount);
  }
  
  if ((render & 3) == 1 || (render & 3) == 2) {
    LOG (printf ("stroke string\n"));
    cairo_set_source (cairo, stroke_pattern);
    cairo_glyph_path (cairo, glyphs, glyphCount);
    cairo_stroke (cairo);
    if (cairo_shape) {
      cairo_glyph_path (cairo_shape, glyphs, glyphCount);
      cairo_stroke (cairo_shape);
    }
  }

  if (render & 4) {
    LOG (printf ("clip string\n"));

    if (textClipPath) {
      cairo_append_path (cairo, textClipPath);
      if (cairo_shape) {
	cairo_append_path (cairo_shape, textClipPath);
      }
      cairo_path_destroy (textClipPath);
    }
    
    cairo_glyph_path (cairo, glyphs, glyphCount);
   
    textClipPath = cairo_copy_path (cairo);
    cairo_new_path (cairo);
    if (cairo_shape) {
      cairo_new_path (cairo_shape);
    }
  }

  gfree (glyphs);
  glyphs = NULL;
}
