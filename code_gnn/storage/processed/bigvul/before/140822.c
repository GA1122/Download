error::Error GLES2DecoderImpl::HandleShaderBinary(uint32 immediate_data_size,
                                                  const void* cmd_data) {
#if 1   
  LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glShaderBinary", "not supported");
  return error::kNoError;
#else
  GLsizei n = static_cast<GLsizei>(c.n);
  if (n < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glShaderBinary", "n < 0");
    return error::kNoError;
  }
  GLsizei length = static_cast<GLsizei>(c.length);
  if (length < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glShaderBinary", "length < 0");
    return error::kNoError;
  }
  uint32 data_size;
  if (!SafeMultiplyUint32(n, sizeof(GLuint), &data_size)) {
    return error::kOutOfBounds;
  }
  const GLuint* shaders = GetSharedMemoryAs<const GLuint*>(
      c.shaders_shm_id, c.shaders_shm_offset, data_size);
  GLenum binaryformat = static_cast<GLenum>(c.binaryformat);
  const void* binary = GetSharedMemoryAs<const void*>(
      c.binary_shm_id, c.binary_shm_offset, length);
  if (shaders == NULL || binary == NULL) {
    return error::kOutOfBounds;
  }
  scoped_ptr<GLuint[]> service_ids(new GLuint[n]);
  for (GLsizei ii = 0; ii < n; ++ii) {
    Shader* shader = GetShader(shaders[ii]);
    if (!shader) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, "glShaderBinary", "unknown shader");
      return error::kNoError;
    }
    service_ids[ii] = shader->service_id();
  }
  return error::kNoError;
#endif
}
