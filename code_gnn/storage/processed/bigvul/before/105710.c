  InputMethodLibraryStubImpl()
      : keyboard_overlay_map_(GetKeyboardOverlayMapForTesting()) {
    current_input_method_ = input_method::GetFallbackInputMethodDescriptor();
  }
