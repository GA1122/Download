void RenderWidgetHostViewAura::InitAsFullscreen(
    RenderWidgetHostView* reference_host_view) {
  is_fullscreen_ = true;
  CreateDelegatedFrameHostClient();
  CreateAuraWindow(ui::wm::WINDOW_TYPE_NORMAL);
  window_->SetProperty(aura::client::kShowStateKey, ui::SHOW_STATE_FULLSCREEN);

  aura::Window* parent = NULL;
  gfx::Rect bounds;
  if (reference_host_view) {
    aura::Window* reference_window =
        static_cast<RenderWidgetHostViewAura*>(reference_host_view)->window_;
    event_handler_->TrackHost(reference_window);
    display::Display display =
        display::Screen::GetScreen()->GetDisplayNearestWindow(reference_window);
    parent = reference_window->GetRootWindow();
    bounds = display.bounds();
  }
  aura::client::ParentWindowWithContext(window_, parent, bounds);
  Show();
  Focus();

  device_scale_factor_ = ui::GetScaleFactorForNativeView(window_);
}
