GLES2DecoderImpl::GetOrCreateTranslator(GLenum type) {
  if (!InitializeShaderTranslator()) {
    return nullptr;
  }
  return GetTranslator(type);
}
