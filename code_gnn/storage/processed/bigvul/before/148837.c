  bool SetSelectionChangeListener() {
    return ExecuteScript(interstitial_->GetMainFrame(),
                         "set_selection_change_listener()");
  }
