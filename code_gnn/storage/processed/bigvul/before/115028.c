void TestingAutomationProvider::SetBooleanPreference(int handle,
                                                     const std::string& name,
                                                     bool value,
                                                     bool* success) {
  *success = false;
  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    browser->profile()->GetPrefs()->SetBoolean(name.c_str(), value);
    *success = true;
  }
}
