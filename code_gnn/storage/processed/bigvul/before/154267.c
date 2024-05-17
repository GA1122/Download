error::Error GLES2DecoderImpl::HandleQueryCounterEXT(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::QueryCounterEXT& c =
      *static_cast<const volatile gles2::cmds::QueryCounterEXT*>(cmd_data);
  GLuint client_id = static_cast<GLuint>(c.id);
  GLenum target = static_cast<GLenum>(c.target);
  int32_t sync_shm_id = static_cast<int32_t>(c.sync_data_shm_id);
  uint32_t sync_shm_offset = static_cast<uint32_t>(c.sync_data_shm_offset);
  uint32_t submit_count = static_cast<GLuint>(c.submit_count);

  switch (target) {
    case GL_TIMESTAMP:
      if (!query_manager_->GPUTimingAvailable()) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, "glQueryCounterEXT",
            "not enabled for timing queries");
        return error::kNoError;
      }
      break;
    default:
      LOCAL_SET_GL_ERROR(
          GL_INVALID_ENUM, "glQueryCounterEXT",
          "unknown query target");
      return error::kNoError;
  }

  scoped_refptr<gpu::Buffer> buffer = GetSharedMemoryBuffer(sync_shm_id);
  if (!buffer)
    return error::kInvalidArguments;
  QuerySync* sync = static_cast<QuerySync*>(
      buffer->GetDataAddress(sync_shm_offset, sizeof(QuerySync)));
  if (!sync)
    return error::kOutOfBounds;

  QueryManager::Query* query = query_manager_->GetQuery(client_id);
  if (!query) {
    if (!query_manager_->IsValidQuery(client_id)) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION,
                         "glQueryCounterEXT",
                         "id not made by glGenQueriesEXT");
      return error::kNoError;
    }
    query =
        query_manager_->CreateQuery(target, client_id, std::move(buffer), sync);
  } else {
    if (query->target() != target) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glQueryCounterEXT",
                         "target does not match");
      return error::kNoError;
    } else if (query->sync() != sync) {
      DLOG(ERROR) << "Shared memory used by query not the same as before";
      return error::kInvalidArguments;
    }
  }
  query_manager_->QueryCounter(query, submit_count);

  return error::kNoError;
}
