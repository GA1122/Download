const GLubyte* GLES2Implementation::GetStringi(GLenum name, GLuint index) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetStringi("
                     << GLES2Util::GetStringStringType(name) << "," << index
                     << ")");
  TRACE_EVENT0("gpu", "GLES2::GetStringi");
  UpdateCachedExtensionsIfNeeded();
  if (name != GL_EXTENSIONS) {
    SetGLError(GL_INVALID_ENUM, "glGetStringi", "name");
    return nullptr;
  }
  if (index >= cached_extensions_.size()) {
    SetGLError(GL_INVALID_VALUE, "glGetStringi", "index too large");
    return nullptr;
  }

  const char* result = cached_extensions_[index];
  GPU_CLIENT_LOG("  returned " << result);
  CheckGLError();
  return reinterpret_cast<const GLubyte*>(result);
}
