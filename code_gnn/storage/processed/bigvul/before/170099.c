    void RemoveActiveInputMethodId(const std::string& ime_id) {
      base::EraseIf(active_input_method_ids_,
                    [&ime_id](const std::string& id) { return id == ime_id; });
    }
