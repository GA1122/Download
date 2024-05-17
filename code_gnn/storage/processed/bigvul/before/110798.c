void AutocompleteEditModel::OnControlKeyChanged(bool pressed) {
  if (pressed == (control_key_state_ == UP)) {
    ControlKeyState old_state = control_key_state_;
    control_key_state_ = pressed ? DOWN_WITHOUT_CHANGE : UP;
    if ((control_key_state_ == DOWN_WITHOUT_CHANGE) && has_temporary_text_) {
      InternalSetUserText(UserTextFromDisplayText(view_->GetText()));
      has_temporary_text_ = false;
      if (KeywordIsSelected())
        AcceptKeyword();
    }
    if ((old_state != DOWN_WITH_CHANGE) && popup_->IsOpen()) {
      view_->UpdatePopup();
    }
  }
}
