const GLubyte* GLES2Implementation::GetString(GLenum name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetString("
                     << GLES2Util::GetStringStringType(name) << ")");
  TRACE_EVENT0("gpu", "GLES2::GetString");
  const GLubyte* result = GetStringHelper(name);
  GPU_CLIENT_LOG("  returned " << reinterpret_cast<const char*>(result));
  CheckGLError();
  return result;
}
