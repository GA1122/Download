  bool GetPathNameDataImpl(GLuint num_paths,
                           GLuint path_base,
                           uint32_t shm_id,
                           uint32_t shm_offset,
                           std::unique_ptr<GLuint[]>* out_buffer) {
    uint32_t paths_size = 0;
    if (!SafeMultiplyUint32(num_paths, sizeof(T), &paths_size)) {
      error_ = error::kOutOfBounds;
      return false;
    }
    T* paths = decoder_->GetSharedMemoryAs<T*>(shm_id, shm_offset, paths_size);
    if (!paths) {
      error_ = error::kOutOfBounds;
      return false;
    }
    std::unique_ptr<GLuint[]> result_paths(new GLuint[num_paths]);
    bool has_paths = false;
    for (GLuint i = 0; i < num_paths; ++i) {
      GLuint service_id = 0;
      uint32_t client_id = path_base + paths[i];
      if (decoder_->path_manager()->GetPath(client_id, &service_id))
        has_paths = true;
      result_paths[i] = service_id;
    }
    out_buffer->reset(result_paths.release());

    return has_paths;
  }
