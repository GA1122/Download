void Document::IncrementPasswordCount() {
  ++password_count_;
  if (IsSecureContext() || password_count_ != 1) {
    return;
  }
  SendSensitiveInputVisibility();
}
