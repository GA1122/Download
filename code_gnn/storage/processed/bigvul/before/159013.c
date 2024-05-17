ScopedSubstFont::ScopedSubstFont(PDFiumEngine* engine)
    : old_engine_(g_engine_for_fontmapper) {
  g_engine_for_fontmapper = engine;
}
