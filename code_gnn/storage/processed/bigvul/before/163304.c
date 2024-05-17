void RenderThreadImpl::ClearMemory() {
  if (blink_platform_impl_) {
    size_t font_cache_limit = SkGraphics::SetFontCacheLimit(0);
    SkGraphics::SetFontCacheLimit(font_cache_limit);
  }
}
