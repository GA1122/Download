CanvasFontCache* Document::GetCanvasFontCache() {
  if (!canvas_font_cache_)
    canvas_font_cache_ = CanvasFontCache::Create(*this);

  return canvas_font_cache_.Get();
}
