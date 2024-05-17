bool GLES2DecoderImpl::MakeCurrent() {
  bool result = context_.get() ? context_->MakeCurrent(surface_.get()) : false;
  if (result && WasContextLost()) {
    LOG(ERROR) << "  GLES2DecoderImpl: Context lost during MakeCurrent.";
    result = false;
  }

  return result;
}
