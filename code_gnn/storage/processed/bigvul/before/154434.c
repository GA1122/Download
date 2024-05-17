void GLES2DecoderPassthroughImpl::EmulatedColorBuffer::Destroy(
    bool have_context) {
  if (!have_context) {
    texture->MarkContextLost();
  }
  texture = nullptr;
}
