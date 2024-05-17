void TestingAutomationProvider::SetIntPreference(int handle,
                                                 const std::string& name,
                                                 int value,
                                                 bool* success) {
  *success = false;
  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    browser->profile()->GetPrefs()->SetInteger(name.c_str(), value);
    *success = true;
  }
}
