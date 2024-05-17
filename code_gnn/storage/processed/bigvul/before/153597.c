void GLES2Implementation::CoverFillPathInstancedCHROMIUM(
    GLsizei num_paths,
    GLenum path_name_type,
    const GLvoid* paths,
    GLuint path_base,
    GLenum cover_mode,
    GLenum transform_type,
    const GLfloat* transform_values) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glCoverFillPathInstancedCHROMIUM("
                     << num_paths << ", " << path_name_type << ", " << paths
                     << ", " << path_base << ", " << cover_mode << ", "
                     << transform_type << ", " << transform_values << ")");

  ScopedTransferBufferPtr buffer(helper_, transfer_buffer_);
  uint32_t paths_shm_id = 0;
  uint32_t paths_offset = 0;
  uint32_t transforms_shm_id = 0;
  uint32_t transforms_offset = 0;
  if (!PrepareInstancedPathCommand(
          "glCoverFillPathInstancedCHROMIUM", num_paths, path_name_type, paths,
          transform_type, transform_values, &buffer, &paths_shm_id,
          &paths_offset, &transforms_shm_id, &transforms_offset)) {
    return;
  }

  helper_->CoverFillPathInstancedCHROMIUM(
      num_paths, path_name_type, paths_shm_id, paths_offset, path_base,
      cover_mode, transform_type, transforms_shm_id, transforms_offset);

  CheckGLError();
}
