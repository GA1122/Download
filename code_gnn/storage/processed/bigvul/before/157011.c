bool WebMediaPlayerMS::TexImageImpl(TexImageFunctionID functionID,
                                    unsigned target,
                                    gpu::gles2::GLES2Interface* gl,
                                    unsigned int texture,
                                    int level,
                                    int internalformat,
                                    unsigned format,
                                    unsigned type,
                                    int xoffset,
                                    int yoffset,
                                    int zoffset,
                                    bool flip_y,
                                    bool premultiply_alpha) {
  TRACE_EVENT0("media", "texImageImpl");
  DCHECK(thread_checker_.CalledOnValidThread());

  const scoped_refptr<media::VideoFrame> video_frame =
      compositor_->GetCurrentFrameWithoutUpdatingStatistics();
  if (!video_frame || !video_frame->IsMappable() ||
      video_frame->HasTextures() ||
      video_frame->format() != media::PIXEL_FORMAT_Y16) {
    return false;
  }

  if (functionID == kTexImage2D) {
    auto* provider =
        RenderThreadImpl::current()->SharedMainThreadContextProvider().get();
    if (!provider)
      return false;
    return media::PaintCanvasVideoRenderer::TexImage2D(
        target, texture, gl, provider->ContextCapabilities(), video_frame.get(),
        level, internalformat, format, type, flip_y, premultiply_alpha);
  } else if (functionID == kTexSubImage2D) {
    return media::PaintCanvasVideoRenderer::TexSubImage2D(
        target, gl, video_frame.get(), level, format, type, xoffset, yoffset,
        flip_y, premultiply_alpha);
  }
  return false;
}
