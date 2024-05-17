ScopedSubstFont::~ScopedSubstFont() {
  g_engine_for_fontmapper = old_engine_;
}
