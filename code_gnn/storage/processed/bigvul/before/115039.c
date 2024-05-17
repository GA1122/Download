void TestingAutomationProvider::SetStringPreference(int handle,
                                                    const std::string& name,
                                                    const std::string& value,
                                                    bool* success) {
  *success = false;
  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    browser->profile()->GetPrefs()->SetString(name.c_str(), value);
    *success = true;
  }
}
