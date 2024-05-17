RenderWidgetHostViewAura::AccessibilityGetNativeViewAccessible() {
#if defined(OS_WIN)
  if (legacy_render_widget_host_HWND_)
    return legacy_render_widget_host_HWND_->window_accessible();
#endif
  return NULL;
}
