  ClientChangeVerifier()
     : previous_client_(NULL),
       next_client_(NULL),
       call_expected_(false),
       on_will_change_focused_client_called_(false),
       on_did_change_focused_client_called_(false),
       on_text_input_state_changed_(false) {
  }
