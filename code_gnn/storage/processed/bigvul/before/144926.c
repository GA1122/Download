void RenderWidgetHostViewAura::InitAsChild(
    gfx::NativeView parent_view) {
  CreateDelegatedFrameHostClient();

  CreateAuraWindow(ui::wm::WINDOW_TYPE_CONTROL);

  if (parent_view)
    parent_view->AddChild(GetNativeView());

  device_scale_factor_ = ui::GetScaleFactorForNativeView(window_);
}
