gfx::Rect AeroPeekWindow::GetContentBounds() const {
  RECT content_rect;
  GetClientRect(frame_window_, &content_rect);

  gfx::Insets content_insets;
  delegate_->GetContentInsets(&content_insets);

  gfx::Rect content_bounds(content_rect);
  content_bounds.Inset(content_insets.left(),
                       content_insets.top(),
                       content_insets.right(),
                       content_insets.bottom());
  return content_bounds;
}
