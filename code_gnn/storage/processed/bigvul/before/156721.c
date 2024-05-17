  explicit TextSelectionObserver(TextInputManager* text_input_manager)
      : text_input_manager_(text_input_manager) {
    text_input_manager->AddObserver(this);
  }
