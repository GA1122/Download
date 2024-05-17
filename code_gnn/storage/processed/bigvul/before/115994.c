void ExtensionGlobalError::BubbleViewCancelButtonPressed() {
  if (!cancel_callback_.is_null()) {
    cancel_callback_.Run(*this, current_browser_);
  }
}
