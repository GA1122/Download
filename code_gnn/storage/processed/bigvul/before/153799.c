void GLES2Implementation::RemoveTransferBuffer(BufferTracker::Buffer* buffer) {
  int32_t token = buffer->last_usage_token();

  if (token) {
    if (helper_->HasTokenPassed(token))
      buffer_tracker_->Free(buffer);
    else
      buffer_tracker_->FreePendingToken(buffer, token);
  } else {
    buffer_tracker_->Free(buffer);
  }

  buffer_tracker_->RemoveBuffer(buffer->id());
}
