GLuint GLES2Implementation::GetMaxValueInBufferCHROMIUM(GLuint buffer_id,
                                                        GLsizei count,
                                                        GLenum type,
                                                        GLuint offset) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetMaxValueInBufferCHROMIUM("
                     << buffer_id << ", " << count << ", "
                     << GLES2Util::GetStringGetMaxIndexType(type) << ", "
                     << offset << ")");
  GLuint result =
      GetMaxValueInBufferCHROMIUMHelper(buffer_id, count, type, offset);
  GPU_CLIENT_LOG("returned " << result);
  CheckGLError();
  return result;
}
