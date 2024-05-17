void RenderWidgetHostViewAura::UpdateMouseLockRegion() {
  RECT window_rect =
      display::Screen::GetScreen()
          ->DIPToScreenRectInWindow(window_, window_->GetBoundsInScreen())
          .ToRECT();
  ::ClipCursor(&window_rect);
}
