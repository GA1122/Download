bool WebGLRenderingContextBase::CopyRenderingResultsFromDrawingBuffer(
    CanvasResourceProvider* resource_provider,
    SourceDrawingBuffer source_buffer) const {
  if (!drawing_buffer_)
    return false;
  base::WeakPtr<WebGraphicsContext3DProviderWrapper> shared_context_wrapper =
      SharedGpuContext::ContextProviderWrapper();
  if (!shared_context_wrapper)
    return false;
  gpu::gles2::GLES2Interface* gl =
      shared_context_wrapper->ContextProvider()->ContextGL();
  GLuint texture_id = resource_provider->GetBackingTextureHandleForOverwrite();
  if (!texture_id)
    return false;

  gl->Flush();

  return drawing_buffer_->CopyToPlatformTexture(
      gl, GL_TEXTURE_2D, texture_id, true, false, IntPoint(0, 0),
      IntRect(IntPoint(0, 0), drawing_buffer_->Size()), source_buffer);
}
