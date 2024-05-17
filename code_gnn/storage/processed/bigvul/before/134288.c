void OmniboxViewViews::OnMouseReleased(const ui::MouseEvent& event) {
  views::Textfield::OnMouseReleased(event);
  if ((event.IsOnlyLeftMouseButton() || event.IsOnlyRightMouseButton()) &&
      select_all_on_mouse_release_ &&
      !controller()->GetToolbarModel()->WouldReplaceURL()) {
    SelectAll(true);
  }
  select_all_on_mouse_release_ = false;
}
