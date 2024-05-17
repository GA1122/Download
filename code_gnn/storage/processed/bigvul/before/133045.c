void RenderWidgetHostViewAura::TextInputTypeChanged(
    ui::TextInputType type,
    ui::TextInputMode input_mode,
    bool can_compose_inline) {
  if (text_input_type_ != type ||
      text_input_mode_ != input_mode ||
      can_compose_inline_ != can_compose_inline) {
    text_input_type_ = type;
    text_input_mode_ = input_mode;
    can_compose_inline_ = can_compose_inline;
    if (GetInputMethod())
      GetInputMethod()->OnTextInputTypeChanged(this);
    if (touch_editing_client_)
      touch_editing_client_->OnTextInputTypeChanged(text_input_type_);
  }
}
