    bool IsInputMethodAllowed(const std::string& ime_id) {
      return allowed_input_methods_.empty() ||
             base::ContainsValue(allowed_input_methods_, ime_id);
    }
