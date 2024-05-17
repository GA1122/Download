  virtual bool Cancel() {
    action_taken_ = true;
    UMA_HISTOGRAM_COUNTS("DefaultBrowserWarning.DontSetAsDefault", 1);
    profile_->GetPrefs()->SetBoolean(prefs::kCheckDefaultBrowser, false);
    return true;
  }
