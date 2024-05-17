void ExtensionGlobalError::BubbleViewAcceptButtonPressed() {
  if (!accept_callback_.is_null()) {
    accept_callback_.Run(*this, current_browser_);
  }
}
