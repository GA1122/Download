bool FolderHeaderView::OnKeyPressed(const ui::KeyEvent& event) {
  if (event.key_code() == ui::VKEY_RETURN)
    delegate_->GiveBackFocusToSearchBox();

  return false;
}
