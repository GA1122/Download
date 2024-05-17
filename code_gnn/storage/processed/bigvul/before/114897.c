void TestingAutomationProvider::GetBooleanPreference(int handle,
                                                     const std::string& name,
                                                     bool* success,
                                                     bool* value) {
  *success = false;
  *value = false;
  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    *value = browser->profile()->GetPrefs()->GetBoolean(name.c_str());
    *success = true;
  }
}
