bool GLES2Implementation::PrepareInstancedPathCommand(
    const char* function_name,
    GLsizei num_paths,
    GLenum path_name_type,
    const void* paths,
    GLenum transform_type,
    const GLfloat* transform_values,
    ScopedTransferBufferPtr* buffer,
    uint32_t* out_paths_shm_id,
    uint32_t* out_paths_offset,
    uint32_t* out_transforms_shm_id,
    uint32_t* out_transforms_offset) {
  if (num_paths < 0) {
    SetGLError(GL_INVALID_VALUE, function_name, "numPaths < 0");
    return false;
  }
  uint32_t path_name_size =
      GLES2Util::GetGLTypeSizeForGLPathNameType(path_name_type);

  if (path_name_size == 0) {
    SetGLError(GL_INVALID_ENUM, function_name, "invalid pathNameType");
    return false;
  }

  uint32_t transforms_component_count =
      GLES2Util::GetComponentCountForGLTransformType(transform_type);

  if (transform_type != GL_NONE && transforms_component_count == 0) {
    SetGLError(GL_INVALID_ENUM, function_name, "invalid transformType");
    return false;
  }

  if (num_paths == 0) {
    *out_paths_shm_id = 0;
    *out_paths_offset = 0;
    *out_transforms_shm_id = 0;
    *out_transforms_offset = 0;
    return true;
  }

  if (!paths) {
    SetGLError(GL_INVALID_VALUE, function_name, "missing paths");
    return false;
  }

  if (transform_type != GL_NONE && !transform_values) {
    SetGLError(GL_INVALID_VALUE, function_name, "missing transforms");
    return false;
  }

  uint32_t paths_size;
  if (!base::CheckMul(path_name_size, num_paths).AssignIfValid(&paths_size)) {
    SetGLError(GL_INVALID_OPERATION, function_name, "overflow");
    return false;
  }

  DCHECK_LE(transforms_component_count, 12U);
  uint32_t one_transform_size = sizeof(GLfloat) * transforms_component_count;

  uint32_t transforms_size;
  if (!base::CheckMul(one_transform_size, num_paths)
           .AssignIfValid(&transforms_size)) {
    SetGLError(GL_INVALID_OPERATION, function_name, "overflow");
    return false;
  }

  uint32_t required_buffer_size;
  if (!base::CheckAdd(transforms_size, paths_size)
           .AssignIfValid(&required_buffer_size)) {
    SetGLError(GL_INVALID_OPERATION, function_name, "overflow");
    return false;
  }

  buffer->Reset(required_buffer_size);

  if (!buffer->valid() || buffer->size() < required_buffer_size) {
    SetGLError(GL_OUT_OF_MEMORY, function_name, "too large");
    return false;
  }

  if (transforms_size > 0) {
    unsigned char* transforms_addr =
        static_cast<unsigned char*>(buffer->address());
    memcpy(transforms_addr, transform_values, transforms_size);
    *out_transforms_shm_id = buffer->shm_id();
    *out_transforms_offset = buffer->offset();
  } else {
    *out_transforms_shm_id = 0;
    *out_transforms_offset = 0;
  }

  DCHECK_GT(paths_size, 0U);
  unsigned char* paths_addr =
      static_cast<unsigned char*>(buffer->address()) + transforms_size;
  memcpy(paths_addr, paths, paths_size);
  *out_paths_shm_id = buffer->shm_id();
  *out_paths_offset = buffer->offset() + transforms_size;

  return true;
}
