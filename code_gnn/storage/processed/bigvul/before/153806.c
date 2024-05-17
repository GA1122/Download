void GLES2Implementation::SendErrorMessage(std::string message, int32_t id) {
  if (error_message_callback_.is_null())
    return;

  if (deferring_error_callbacks_) {
    deferred_error_callbacks_.emplace_back(std::move(message), id);
    return;
  }

  error_message_callback_.Run(message.c_str(), id);
}
