gfx::NativeViewId RenderWidgetHostViewAura::GetNativeViewId() const {
#if defined(OS_WIN)
  aura::WindowEventDispatcher* dispatcher = window_->GetDispatcher();
  if (dispatcher)
    return reinterpret_cast<gfx::NativeViewId>(
        dispatcher->host()->GetAcceleratedWidget());
#endif
  return static_cast<gfx::NativeViewId>(NULL);
}
