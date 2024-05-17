void BrowserViewRenderer::UpdateMemoryPolicy() {
  if (!hardware_enabled_) {
    compositor_->SetMemoryPolicy(0u);
    return;
  }

  size_t bytes_limit = 0u;
  if (g_memory_override_in_bytes) {
    bytes_limit = static_cast<size_t>(g_memory_override_in_bytes);
  } else {
    ParentCompositorDrawConstraints parent_draw_constraints =
        shared_renderer_state_.GetParentDrawConstraintsOnUI();
    gfx::Rect interest_rect =
        offscreen_pre_raster_ || parent_draw_constraints.is_layer
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
