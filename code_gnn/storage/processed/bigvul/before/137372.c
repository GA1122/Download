  TextfieldTest()
      : widget_(NULL),
        textfield_(NULL),
        model_(NULL),
        input_method_(NULL),
        on_before_user_action_(0),
        on_after_user_action_(0),
        copied_to_clipboard_(ui::CLIPBOARD_TYPE_LAST) {
    input_method_ = new MockInputMethod();
    ui::SetUpInputMethodForTesting(input_method_);
  }
