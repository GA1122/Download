error::Error GLES2DecoderImpl::HandleEndQueryEXT(uint32 immediate_data_size,
                                                 const void* cmd_data) {
  const gles2::cmds::EndQueryEXT& c =
      *static_cast<const gles2::cmds::EndQueryEXT*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  uint32 submit_count = static_cast<GLuint>(c.submit_count);
  ContextState::QueryMap::iterator it = state_.current_queries.find(target);

  if (it == state_.current_queries.end()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glEndQueryEXT", "No active query");
    return error::kNoError;
  }

  QueryManager::Query* query = it->second.get();
  if (!query_manager_->EndQuery(query, submit_count)) {
    return error::kOutOfBounds;
  }

  query_manager_->ProcessPendingTransferQueries();

  state_.current_queries.erase(it);
  return error::kNoError;
}
