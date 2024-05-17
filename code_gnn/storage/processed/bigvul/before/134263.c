bool OmniboxViewViews::HandleKeyEvent(views::Textfield* textfield,
                                      const ui::KeyEvent& event) {
  delete_at_end_pressed_ = false;

  if (event.key_code() == ui::VKEY_BACK) {
    if (model()->is_keyword_hint() || model()->keyword().empty() ||
        HasSelection() || GetCursorPosition() != 0)
      return false;
    model()->ClearKeyword(text());
    return true;
  }

  if (event.key_code() == ui::VKEY_DELETE && !event.IsAltDown()) {
    delete_at_end_pressed_ =
        (!HasSelection() && GetCursorPosition() == text().length());
  }

  if (GetCursorPosition() == text().length()) {
    base::i18n::TextDirection direction = GetTextDirection();
    if ((direction == base::i18n::LEFT_TO_RIGHT &&
         event.key_code() == ui::VKEY_RIGHT) ||
        (direction == base::i18n::RIGHT_TO_LEFT &&
         event.key_code() == ui::VKEY_LEFT)) {
      return model()->CommitSuggestedText();
    }
  }

  return false;
}
