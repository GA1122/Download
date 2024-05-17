void Unsupported_Handler(UNSUPPORT_INFO*, int type) {
  if (!g_engine_for_unsupported) {
    NOTREACHED();
    return;
  }

  g_engine_for_unsupported->UnsupportedFeature(type);
}
