bool OmniboxViewViews::OnMouseDragged(const ui::MouseEvent& event) {
  select_all_on_mouse_release_ = false;
  return views::Textfield::OnMouseDragged(event);
}
