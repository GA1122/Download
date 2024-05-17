SynchronousCompositorImpl::~SynchronousCompositorImpl() {
  DCHECK(!output_surface_);
  DCHECK(!begin_frame_source_);
  DCHECK(!input_handler_);
}
