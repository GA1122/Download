void RenderWidgetHostViewAura::InitAsPopup(
    RenderWidgetHostView* parent_host_view,
    const gfx::Rect& bounds_in_screen) {
  CreateDelegatedFrameHostClient();

  popup_parent_host_view_ =
      static_cast<RenderWidgetHostViewAura*>(parent_host_view);

  aura::client::TransientWindowClient* transient_window_client =
      aura::client::GetTransientWindowClient();
  RenderWidgetHostViewAura* old_child =
      popup_parent_host_view_->popup_child_host_view_;
  if (old_child) {
    DCHECK(old_child->popup_parent_host_view_ == popup_parent_host_view_);
    if (transient_window_client) {
      transient_window_client->RemoveTransientChild(
        popup_parent_host_view_->window_, old_child->window_);
    }
    old_child->popup_parent_host_view_ = NULL;
  }
  popup_parent_host_view_->SetPopupChild(this);
  CreateAuraWindow(ui::wm::WINDOW_TYPE_MENU);

  if (transient_window_client) {
    transient_window_client->AddTransientChild(
        popup_parent_host_view_->window_, window_);
  }

  aura::Window* root = popup_parent_host_view_->window_->GetRootWindow();
  aura::client::ParentWindowWithContext(window_, root, bounds_in_screen);

  SetBounds(bounds_in_screen);
  Show();
  if (NeedsMouseCapture())
    window_->SetCapture();

  event_filter_for_popup_exit_.reset(new EventFilterForPopupExit(this));

  device_scale_factor_ = ui::GetScaleFactorForNativeView(window_);
}
