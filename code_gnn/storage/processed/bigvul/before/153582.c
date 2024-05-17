void GLES2Implementation::CallDeferredErrorCallbacks() {
  if (deferred_error_callbacks_.empty())
    return;

  if (error_message_callback_.is_null()) {
    deferred_error_callbacks_.clear();
    return;
  }

  std::deque<DeferredErrorCallback> local_callbacks;
  std::swap(deferred_error_callbacks_, local_callbacks);
  for (auto c : local_callbacks) {
    error_message_callback_.Run(c.message.c_str(), c.id);
  }
}
