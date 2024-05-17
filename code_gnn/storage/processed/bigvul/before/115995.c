void ExtensionGlobalError::BubbleViewDidClose() {
  if (!closed_callback_.is_null()) {
    closed_callback_.Run(*this, current_browser_);
  }
  if (should_delete_self_on_close_) {
    delete this;
  }
}
