static size_t BitmapSizeForPluginRect(const gfx::Rect& plugin_rect) {
  const size_t stride =
      skia::PlatformCanvas::StrideForWidth(plugin_rect.width());
  return stride * plugin_rect.height();
}
