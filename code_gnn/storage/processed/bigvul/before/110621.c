void GLES2DecoderImpl::ReleaseCurrent() {
  if (context_.get())
    context_->ReleaseCurrent(surface_.get());
}
