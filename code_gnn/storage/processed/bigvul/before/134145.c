void InputMethodLinuxX11::Init(bool focused) {
  CHECK(LinuxInputMethodContextFactory::instance());
  input_method_context_ =
      LinuxInputMethodContextFactory::instance()->CreateInputMethodContext(
          this);
  CHECK(input_method_context_.get());

  InputMethodBase::Init(focused);

  if (focused) {
    input_method_context_->OnTextInputTypeChanged(
        GetTextInputClient() ?
        GetTextInputClient()->GetTextInputType() :
        TEXT_INPUT_TYPE_TEXT);
  }
}
