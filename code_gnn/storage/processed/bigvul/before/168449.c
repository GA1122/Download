PlatformFontSkia::PlatformFontSkia() {
  CHECK(InitDefaultFont()) << "Could not find the default font";
  InitFromPlatformFont(g_default_font.Get().get());
}
