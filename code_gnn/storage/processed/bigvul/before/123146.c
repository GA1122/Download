 void RenderWidgetHostViewAura::AddingToRootWindow() {
  host_->ParentChanged(GetNativeViewId());
  UpdateScreenInfo(window_);
}
