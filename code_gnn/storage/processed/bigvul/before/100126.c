void BrowserActionButton::OnMouseReleased(const views::MouseEvent& e,
                                          bool canceled) {
  if (IsPopup() || showing_context_menu_) {
    MenuButton::OnMouseReleased(e, canceled);
  } else {
    TextButton::OnMouseReleased(e, canceled);
  }
}
