void LocalFrame::ResumeSubresourceLoading() {
  pause_handle_bindings_.CloseAllBindings();
}
