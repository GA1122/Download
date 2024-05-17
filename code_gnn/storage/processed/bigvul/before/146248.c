void WebGLRenderingContextBase::AddCompressedTextureFormat(GLenum format) {
  if (!compressed_texture_formats_.Contains(format))
    compressed_texture_formats_.push_back(format);
}
