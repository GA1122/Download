bool GLES2DecoderImpl::MakeCurrent() {
  if (!context_.get() || !context_->MakeCurrent(surface_.get()))
    return false;

  if (WasContextLost()) {
    LOG(ERROR) << "  GLES2DecoderImpl: Context lost during MakeCurrent.";
    return false;
  }

  return true;
}
