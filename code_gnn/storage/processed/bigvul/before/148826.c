  bool FocusInputAndSelectText() {
    return ExecuteScript(interstitial_->GetMainFrame(), "focus_select_input()");
  }
