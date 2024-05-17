void Textfield::UpdateCursorViewPosition() {
  gfx::Rect location(GetRenderText()->GetUpdatedCursorBounds());
  location.set_x(GetMirroredXForRect(location));
  location.set_height(
      std::min(location.height(),
               GetLocalBounds().height() - location.y() - location.y()));
  cursor_view_.SetBoundsRect(location);
}
