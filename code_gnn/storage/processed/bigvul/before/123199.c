void RenderWidgetHostViewAura::InitAsPopup(
    RenderWidgetHostView* parent_host_view,
    const gfx::Rect& bounds_in_screen) {
  popup_parent_host_view_ =
      static_cast<RenderWidgetHostViewAura*>(parent_host_view);

  RenderWidgetHostViewAura* old_child =
      popup_parent_host_view_->popup_child_host_view_;
  if (old_child) {
    DCHECK(old_child->popup_parent_host_view_ == popup_parent_host_view_);
    old_child->popup_parent_host_view_ = NULL;
  }
  popup_parent_host_view_->popup_child_host_view_ = this;
  window_->SetType(aura::client::WINDOW_TYPE_MENU);
  window_->Init(ui::LAYER_TEXTURED);
  window_->SetName("RenderWidgetHostViewAura");

  aura::RootWindow* root = popup_parent_host_view_->window_->GetRootWindow();
  window_->SetDefaultParentByRootWindow(root, bounds_in_screen);

  aura::client::ScreenPositionClient* screen_position_client =
      aura::client::GetScreenPositionClient(root);
  gfx::Point origin_in_parent(bounds_in_screen.origin());
  if (screen_position_client) {
    screen_position_client->ConvertPointFromScreen(
        window_->parent(), &origin_in_parent);
  }
  SetBounds(gfx::Rect(origin_in_parent, bounds_in_screen.size()));
  Show();
}
