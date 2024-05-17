void RenderWidgetHostViewAura::OnHostMovedInPixels(
    const aura::WindowTreeHost* host,
    const gfx::Point& new_origin_in_pixels) {
  TRACE_EVENT1("ui", "RenderWidgetHostViewAura::OnHostMovedInPixels",
               "new_origin_in_pixels", new_origin_in_pixels.ToString());

  UpdateScreenInfo(window_);
}
