void GLES2Implementation::IssueBeginQuery(GLenum target,
                                          GLuint id,
                                          uint32_t sync_data_shm_id,
                                          uint32_t sync_data_shm_offset) {
  helper_->BeginQueryEXT(target, id, sync_data_shm_id, sync_data_shm_offset);
}
