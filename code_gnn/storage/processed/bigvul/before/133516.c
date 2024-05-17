scoped_ptr<OverscrollGlow> CreateGlowEffect(OverscrollGlowClient* client,
                                            float dpi_scale) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableOverscrollEdgeEffect)) {
    return nullptr;
  }

  return make_scoped_ptr(new OverscrollGlow(client));
}
