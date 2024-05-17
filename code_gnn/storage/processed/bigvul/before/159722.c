bool WebGLRenderingContextBase::CopyRenderingResultsFromDrawingBuffer(
    AcceleratedImageBufferSurface* webgl_buffer,
    SourceDrawingBuffer source_buffer) const {
  if (!drawing_buffer_)
    return false;
  std::unique_ptr<WebGraphicsContext3DProvider> provider =
      Platform::Current()->CreateSharedOffscreenGraphicsContext3DProvider();
  if (!provider)
    return false;
  gpu::gles2::GLES2Interface* gl = provider->ContextGL();
  GLuint texture_id = webgl_buffer->GetBackingTextureHandleForOverwrite();
  if (!texture_id)
    return false;

  gl->Flush();

  return drawing_buffer_->CopyToPlatformTexture(
      gl, GL_TEXTURE_2D, texture_id, true, false, IntPoint(0, 0),
      IntRect(IntPoint(0, 0), drawing_buffer_->Size()), source_buffer);
}
