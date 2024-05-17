gfx::NativeViewId RenderWidgetHostViewAura::GetNativeViewId() const {
#if defined(OS_WIN)
  HWND window = window_->GetRootWindow()->GetAcceleratedWidget();
  return reinterpret_cast<gfx::NativeViewId>(window);
#else
  return static_cast<gfx::NativeViewId>(NULL);
#endif
}
