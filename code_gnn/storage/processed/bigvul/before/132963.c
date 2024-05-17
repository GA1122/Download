void RenderWidgetHostViewAura::InitAsChild(
    gfx::NativeView parent_view) {
  window_->SetType(ui::wm::WINDOW_TYPE_CONTROL);
  window_->Init(aura::WINDOW_LAYER_TEXTURED);
  window_->SetName("RenderWidgetHostViewAura");
}
