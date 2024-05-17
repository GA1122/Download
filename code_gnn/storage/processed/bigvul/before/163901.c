bool ImageCapture::HasPendingActivity() const {
  return GetExecutionContext() && HasEventListeners();
}
