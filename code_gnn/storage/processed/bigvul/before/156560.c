  bool has_web_ui_bindings() const {
    return enabled_bindings_ & kWebUIBindingsPolicyMask;
  }
