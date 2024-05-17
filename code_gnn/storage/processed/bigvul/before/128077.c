void SynchronousCompositorImpl::DidInitializeRendererObjects(
    SynchronousCompositorOutputSurface* output_surface,
    SynchronousCompositorExternalBeginFrameSource* begin_frame_source,
    SynchronousInputHandlerProxy* synchronous_input_handler_proxy) {
  DCHECK(!output_surface_);
  DCHECK(!begin_frame_source_);
  DCHECK(output_surface);
  DCHECK(begin_frame_source);
  DCHECK(synchronous_input_handler_proxy);

  output_surface_ = output_surface;
  begin_frame_source_ = begin_frame_source;
  synchronous_input_handler_proxy_ = synchronous_input_handler_proxy;

  output_surface_->SetSyncClient(this);
  begin_frame_source_->SetClient(this);
}
