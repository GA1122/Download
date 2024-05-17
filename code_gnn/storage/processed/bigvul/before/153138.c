ScopedUnsupportedFeature::~ScopedUnsupportedFeature() {
  g_engine_for_unsupported = old_engine_;
}
