void GLManager::MakeCurrent() {
  ::gles2::SetGLContext(gles2_implementation_.get());
  if (!decoder_->MakeCurrent())
    command_buffer_->service()->SetParseError(error::kLostContext);
}
