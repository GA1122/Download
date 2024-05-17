void Textfield::InsertChar(const ui::KeyEvent& event) {
  if (read_only()) {
    OnEditFailed();
    return;
  }

  const base::char16 ch = event.GetCharacter();
  const bool should_insert_char = ((ch >= 0x20 && ch < 0x7F) || ch > 0x9F) &&
                                  !ui::IsSystemKeyModifier(event.flags()) &&
                                  !IsControlKeyModifier(event.flags());
  if (GetTextInputType() == ui::TEXT_INPUT_TYPE_NONE || !should_insert_char)
    return;

  DoInsertChar(ch);

  if (text_input_type_ == ui::TEXT_INPUT_TYPE_PASSWORD &&
      !GetPasswordRevealDuration().is_zero()) {
    const size_t change_offset = model_->GetCursorPosition();
    DCHECK_GT(change_offset, 0u);
    RevealPasswordChar(change_offset - 1);
  }
}
