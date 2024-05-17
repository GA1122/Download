CrosMock::CrosMock()
    : loader_(NULL),
      mock_cryptohome_library_(NULL),
      mock_input_method_library_(NULL),
      mock_network_library_(NULL),
      mock_power_library_(NULL),
      mock_screen_lock_library_(NULL),
      mock_speech_synthesis_library_(NULL),
      mock_touchpad_library_(NULL) {
  current_input_method_ =
      input_method::GetFallbackInputMethodDescriptor();
}