void BrowserActionButton::OnMouseExited(const views::MouseEvent& e) {
  if (IsPopup() || showing_context_menu_)
    MenuButton::OnMouseExited(e);
  else
    TextButton::OnMouseExited(e);
}
