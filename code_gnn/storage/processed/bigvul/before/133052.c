void RenderWidgetHostViewAura::UpdateCutoutRects() {
  if (!window_->GetRootWindow())
    return;
  HWND parent = window_->GetDispatcher()->host()->GetAcceleratedWidget();
  CutoutRectsParams params;
  params.widget = this;
  params.cutout_rects = constrained_rects_;
  params.geometry = &plugin_window_moves_;
  LPARAM lparam = reinterpret_cast<LPARAM>(&params);
  EnumChildWindows(parent, SetCutoutRectsCallback, lparam);
}
