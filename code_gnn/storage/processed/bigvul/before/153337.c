bool Tab::OnKeyPressed(const ui::KeyEvent& event) {
  controller_->UpdateHoverCard(this, false);
  if (event.key_code() == ui::VKEY_SPACE && !IsSelected()) {
    controller_->SelectTab(this, event);
    return true;
  }

  return false;
}
