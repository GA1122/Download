void BrowserViewRenderer::UpdateMemoryPolicy() {
  if (!hardware_enabled_) {
    compositor_->SetMemoryPolicy(0u);
    return;
  }

  size_t bytes_limit = 0u;
  if (g_memory_override_in_bytes) {
    bytes_limit = static_cast<size_t>(g_memory_override_in_bytes);
  } else {
    gfx::Rect interest_rect = offscreen_pre_raster_
                                  ? gfx::Rect(size_)
                                  : last_on_draw_global_visible_rect_;
    size_t width = interest_rect.width();
    size_t height = interest_rect.height();
    bytes_limit = kMemoryMultiplier * kBytesPerPixel * width * height;
    bytes_limit =
        (bytes_limit / kMemoryAllocationStep + 1) * kMemoryAllocationStep;
  }

  compositor_->SetMemoryPolicy(bytes_limit);
}
