bool SynchronousCompositorImpl::DemandDrawSw(SkCanvas* canvas) {
  DCHECK(CalledOnValidThread());
  DCHECK(output_surface_);
  DCHECK(compositor_client_);
  DCHECK(begin_frame_source_);

  scoped_ptr<cc::CompositorFrame> frame =
      output_surface_->DemandDrawSw(canvas);

  if (frame.get())
    UpdateFrameMetaData(frame->metadata);

  return !!frame.get();
}
