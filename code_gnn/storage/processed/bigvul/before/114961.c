void TestingAutomationProvider::GetType(int handle, int* type_as_int) {
  *type_as_int = -1;   

  if (browser_tracker_->ContainsHandle(handle)) {
    Browser* browser = browser_tracker_->GetResource(handle);
    *type_as_int = static_cast<int>(browser->type());
  }
}
