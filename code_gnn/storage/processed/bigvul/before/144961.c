void RenderWidgetHostViewAura::OnWindowDestroying(aura::Window* window) {
#if defined(OS_WIN)
  if (legacy_render_widget_host_HWND_) {
    legacy_render_widget_host_HWND_->set_host(NULL);
    legacy_render_widget_host_HWND_->Destroy();
    legacy_render_widget_host_HWND_ = NULL;
  }
#endif

  DetachFromInputMethod();

  if (overscroll_controller_)
    overscroll_controller_->Reset();
}
