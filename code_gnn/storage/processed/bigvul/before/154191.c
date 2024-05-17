scoped_refptr<ShaderTranslatorInterface> GLES2DecoderImpl::GetTranslator(
    GLenum type) {
  return type == GL_VERTEX_SHADER ? vertex_translator_ : fragment_translator_;
}
