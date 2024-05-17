RenderWidgetHostViewAura::AccessibilityGetAcceleratedWidget() {
#if defined(OS_WIN)
  if (legacy_render_widget_host_HWND_)
    return legacy_render_widget_host_HWND_->hwnd();
#endif
  return gfx::kNullAcceleratedWidget;
}
