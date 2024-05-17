bool WebGLRenderingContextBase::CopyRenderingResultsFromDrawingBuffer(
    CanvasResourceProvider* resource_provider,
    SourceDrawingBuffer source_buffer) const {
  if (!drawing_buffer_)
    return false;
  if (resource_provider->IsAccelerated()) {
    base::WeakPtr<WebGraphicsContext3DProviderWrapper> shared_context_wrapper =
        SharedGpuContext::ContextProviderWrapper();
    if (!shared_context_wrapper)
      return false;
    gpu::gles2::GLES2Interface* gl =
        shared_context_wrapper->ContextProvider()->ContextGL();
    GLuint texture_id =
        resource_provider->GetBackingTextureHandleForOverwrite();
    if (!texture_id)
      return false;

    gl->Flush();

    bool flip_y = is_origin_top_left_ && !canvas()->LowLatencyEnabled();
    return drawing_buffer_->CopyToPlatformTexture(
        gl, GL_TEXTURE_2D, texture_id, 0  , true, flip_y,
        IntPoint(0, 0), IntRect(IntPoint(0, 0), drawing_buffer_->Size()),
        source_buffer);
  }

  scoped_refptr<StaticBitmapImage> image = GetImage(kPreferAcceleration);
  if (!image)
    return false;
  cc::PaintFlags paint_flags;
  paint_flags.setBlendMode(SkBlendMode::kSrc);
  resource_provider->Canvas()->drawImage(image->PaintImageForCurrentFrame(), 0,
                                         0, &paint_flags);
  return true;
}
