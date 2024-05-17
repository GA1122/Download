bool WebGraphicsContext3DCommandBufferImpl::makeContextCurrent() {
  if (!MaybeInitializeGL(NULL))
    return false;
  gles2::SetGLContext(gl_);
  if (command_buffer_->GetLastError() != gpu::error::kNoError)
    return false;

  return true;
}
