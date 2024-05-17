  bool GetSelection(std::string* input_text) {
    return ExecuteScriptAndExtractString(interstitial_->GetMainFrame(),
                                         "get_selection()", input_text);
  }
