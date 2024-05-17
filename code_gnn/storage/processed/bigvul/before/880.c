CairoOutputDev::CairoOutputDev() {
  xref = NULL;
  catalog = NULL;

  if (!ft_lib_initialized) {
    FT_Init_FreeType(&ft_lib);
    ft_lib_initialized = gTrue;
  }

  fontEngine = NULL;
  fontEngine_owner = gFalse;
  glyphs = NULL;
  fill_pattern = NULL;
  stroke_pattern = NULL;
  stroke_opacity = 1.0;
  fill_opacity = 1.0;
  textClipPath = NULL;
  cairo = NULL;
  currentFont = NULL;
  prescaleImages = gTrue;
  printing = gTrue;
  inType3Char = gFalse;
  t3_glyph_has_bbox = gFalse;

  groupColorSpaceStack = NULL;
  maskStack = NULL;
  group = NULL;
  mask = NULL;
  shape = NULL;
  cairo_shape = NULL;
  knockoutCount = 0;

  text = NULL;
  actualText = NULL;
}
