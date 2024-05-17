WebKeyEvent::WebKeyEvent(automation::KeyEventTypes type,
                         ui::KeyboardCode key_code,
                         const std::string& unmodified_text,
                         const std::string& modified_text,
                         int modifiers)
    : type(type),
      key_code(key_code),
      unmodified_text(unmodified_text),
      modified_text(modified_text),
      modifiers(modifiers) {}
