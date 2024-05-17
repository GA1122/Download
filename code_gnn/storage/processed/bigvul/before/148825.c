  bool CreateInputAndSetText(const std::string& text) {
    return ExecuteScript(interstitial_->GetMainFrame(),
                         "create_input_and_set_text('" + text + "')");
  }
