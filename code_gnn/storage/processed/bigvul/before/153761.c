void GLES2Implementation::IssueQueryCounter(GLuint id,
                                            GLenum target,
                                            uint32_t sync_data_shm_id,
                                            uint32_t sync_data_shm_offset,
                                            GLuint submit_count) {
  helper_->QueryCounterEXT(id, target, sync_data_shm_id, sync_data_shm_offset,
                           submit_count);
}
