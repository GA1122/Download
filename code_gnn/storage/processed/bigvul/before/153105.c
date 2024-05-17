ScopedUnsupportedFeature::ScopedUnsupportedFeature(PDFiumEngine* engine)
    : old_engine_(g_engine_for_unsupported) {
  g_engine_for_unsupported = engine;
}
