void GLES2Implementation::BufferData(GLenum target,
                                     GLsizeiptr size,
                                     const void* data,
                                     GLenum usage) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glBufferData("
                     << GLES2Util::GetStringBufferTarget(target) << ", " << size
                     << ", " << static_cast<const void*>(data) << ", "
                     << GLES2Util::GetStringBufferUsage(usage) << ")");
  BufferDataHelper(target, size, data, usage);
  CheckGLError();
}
