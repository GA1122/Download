  void set_custom_visible_security_state(
      std::unique_ptr<VisibleSecurityState> visible_security_state) {
    custom_visible_security_state_ = std::move(visible_security_state);
  }
