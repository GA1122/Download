  bool GetPathNameData(const Cmd& cmd,
                       GLuint num_paths,
                       GLenum path_name_type,
                       std::unique_ptr<GLuint[]>* out_buffer) {
    DCHECK(validators_->path_name_type.IsValid(path_name_type));
    GLuint path_base = static_cast<GLuint>(cmd.pathBase);
    uint32_t shm_id = static_cast<uint32_t>(cmd.paths_shm_id);
    uint32_t shm_offset = static_cast<uint32_t>(cmd.paths_shm_offset);
    if (shm_id == 0 && shm_offset == 0) {
      error_ = error::kOutOfBounds;
      return false;
    }
    switch (path_name_type) {
      case GL_BYTE:
        return GetPathNameDataImpl<GLbyte>(num_paths, path_base, shm_id,
                                           shm_offset, out_buffer);
      case GL_UNSIGNED_BYTE:
        return GetPathNameDataImpl<GLubyte>(num_paths, path_base, shm_id,
                                            shm_offset, out_buffer);
      case GL_SHORT:
        return GetPathNameDataImpl<GLshort>(num_paths, path_base, shm_id,
                                            shm_offset, out_buffer);
      case GL_UNSIGNED_SHORT:
        return GetPathNameDataImpl<GLushort>(num_paths, path_base, shm_id,
                                             shm_offset, out_buffer);
      case GL_INT:
        return GetPathNameDataImpl<GLint>(num_paths, path_base, shm_id,
                                          shm_offset, out_buffer);
      case GL_UNSIGNED_INT:
        return GetPathNameDataImpl<GLuint>(num_paths, path_base, shm_id,
                                           shm_offset, out_buffer);
      default:
        break;
    }
    NOTREACHED();
    error_ = error::kOutOfBounds;
    return false;
  }
