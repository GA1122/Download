void Textfield::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  gfx::Rect bounds = GetLocalBounds();
  const gfx::Insets insets = GetInsets();
  bounds.Inset(insets.left(), 0, insets.right(), 0);
  GetRenderText()->SetDisplayRect(bounds);
  OnCaretBoundsChanged();
  UpdateCursorViewPosition();
  UpdateCursorVisibility();
}
