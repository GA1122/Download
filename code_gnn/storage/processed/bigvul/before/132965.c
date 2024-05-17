void RenderWidgetHostViewAura::InitAsPopup(
    RenderWidgetHostView* parent_host_view,
    const gfx::Rect& bounds_in_screen) {
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
  popup_parent_host_view_->popup_child_host_view_ = this;
  window_->SetType(ui::wm::WINDOW_TYPE_MENU);
  window_->Init(aura::WINDOW_LAYER_TEXTURED);
  window_->SetName("RenderWidgetHostViewAura");

  aura::Window* root = popup_parent_host_view_->window_->GetRootWindow();
  aura::client::ParentWindowWithContext(window_, root, bounds_in_screen);
  if (transient_window_client) {
    transient_window_client->AddTransientChild(
        popup_parent_host_view_->window_, window_);
  }

  SetBounds(bounds_in_screen);
  Show();
#if !defined(OS_WIN) && !defined(OS_CHROMEOS)
  if (NeedsInputGrab())
    window_->SetCapture();
#endif
}
