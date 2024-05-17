void GLES2Implementation::CopyBufferSubData(GLenum readtarget,
                                            GLenum writetarget,
                                            GLintptr readoffset,
                                            GLintptr writeoffset,
                                            GLsizeiptr size) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glCopyBufferSubData("
                     << GLES2Util::GetStringBufferTarget(readtarget) << ", "
                     << GLES2Util::GetStringBufferTarget(writetarget) << ", "
                     << readoffset << ", " << writeoffset << ", " << size
                     << ")");
  if (readoffset < 0) {
    SetGLError(GL_INVALID_VALUE, "glCopyBufferSubData", "readoffset < 0");
    return;
  }
  if (writeoffset < 0) {
    SetGLError(GL_INVALID_VALUE, "glCopyBufferSubData", "writeoffset < 0");
    return;
  }
  if (size < 0) {
    SetGLError(GL_INVALID_VALUE, "glCopyBufferSubData", "size < 0");
    return;
  }
  helper_->CopyBufferSubData(readtarget, writetarget, readoffset, writeoffset,
                             size);
  InvalidateReadbackBufferShadowDataCHROMIUM(GetBoundBufferHelper(writetarget));
  CheckGLError();
}
