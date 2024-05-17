void Instance::FormTextFieldFocusChange(bool in_focus) {
  if (!text_input_.get())
    return;
  if (in_focus)
    text_input_->SetTextInputType(PP_TEXTINPUT_TYPE_DEV_TEXT);
  else
    text_input_->SetTextInputType(PP_TEXTINPUT_TYPE_DEV_NONE);
}
