void ExtensionGlobalError::set_closed_callback(
    ExtensionGlobalErrorCallback callback) {
  cancel_callback_ = callback;
}
