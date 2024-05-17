bool OmniboxViewViews::OnKeyReleased(const ui::KeyEvent& event) {
  if (event.key_code() == ui::VKEY_CONTROL)
    model()->OnControlKeyChanged(false);
  return views::Textfield::OnKeyReleased(event);
}
