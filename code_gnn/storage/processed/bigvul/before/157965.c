void RenderViewImpl::OnUpdateWindowScreenRect(gfx::Rect window_screen_rect) {
  GetWidget()->SetWindowScreenRect(window_screen_rect);
}
