InputConnectionImpl::InputConnectionImpl(
    chromeos::InputMethodEngine* ime_engine,
    ArcInputMethodManagerBridge* imm_bridge,
    int input_context_id)
    : ime_engine_(ime_engine),
      imm_bridge_(imm_bridge),
      input_context_id_(input_context_id),
      binding_(this),
      composing_text_(),
      state_update_timer_() {}
