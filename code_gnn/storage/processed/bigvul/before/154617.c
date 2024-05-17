error::Error GLES2DecoderPassthroughImpl::DoDeleteBuffers(
    GLsizei n,
    const volatile GLuint* buffers) {
  if (n < 0) {
    InsertError(GL_INVALID_VALUE, "n cannot be negative.");
    return error::kNoError;
  }

  std::vector<GLuint> service_ids(n, 0);
  for (GLsizei ii = 0; ii < n; ++ii) {
    GLuint client_id = buffers[ii];

    for (auto& buffer_binding : bound_buffers_) {
      if (buffer_binding.second == client_id) {
        buffer_binding.second = 0;
      }
      resources_->mapped_buffer_map.erase(client_id);
    }

    service_ids[ii] =
        resources_->buffer_id_map.GetServiceIDOrInvalid(client_id);
    resources_->buffer_id_map.RemoveClientID(client_id);

    auto is_the_deleted_buffer = [client_id](const auto& update) {
      return update.first == client_id;
    };
    base::EraseIf(buffer_shadow_updates_, is_the_deleted_buffer);
    for (PendingQuery& pending_query : pending_queries_) {
      base::EraseIf(pending_query.buffer_shadow_updates, is_the_deleted_buffer);
    }
  }
  api()->glDeleteBuffersARBFn(n, service_ids.data());

  return error::kNoError;
}
