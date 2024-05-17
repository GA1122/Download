bool VRDisplay::HasPendingActivity() const {
  return GetExecutionContext() && HasEventListeners();
}
