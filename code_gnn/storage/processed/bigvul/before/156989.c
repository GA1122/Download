void WebMediaPlayerMS::Paint(cc::PaintCanvas* canvas,
                             const blink::WebRect& rect,
                             cc::PaintFlags& flags,
                             int already_uploaded_id,
                             VideoFrameUploadMetadata* out_metadata) {
  DVLOG(3) << __func__;
  DCHECK(thread_checker_.CalledOnValidThread());

  const scoped_refptr<media::VideoFrame> frame =
      compositor_->GetCurrentFrameWithoutUpdatingStatistics();

  media::Context3D context_3d;
  gpu::ContextSupport* context_support = nullptr;
  if (frame && frame->HasTextures()) {
    auto* provider =
        RenderThreadImpl::current()->SharedMainThreadContextProvider().get();
    if (!provider)
      return;
    context_3d = media::Context3D(provider->ContextGL(), provider->GrContext());
    context_support = provider->ContextSupport();
  }
  const gfx::RectF dest_rect(rect.x, rect.y, rect.width, rect.height);
  video_renderer_.Paint(frame, canvas, dest_rect, flags, video_rotation_,
                         context_3d, context_support);
 }
