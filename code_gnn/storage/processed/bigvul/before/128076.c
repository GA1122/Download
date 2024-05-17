void SynchronousCompositorImpl::DidDestroyRendererObjects() {
  DCHECK(output_surface_);
  DCHECK(begin_frame_source_);

  if (registered_with_client_) {
    output_surface_->SetTreeActivationCallback(base::Closure());
    compositor_client_->DidDestroyCompositor(this);
    registered_with_client_ = false;
  }

  begin_frame_source_->SetClient(nullptr);
  output_surface_->SetSyncClient(nullptr);
  synchronous_input_handler_proxy_->SetOnlySynchronouslyAnimateRootFlings(
      nullptr);

  synchronous_input_handler_proxy_ = nullptr;
  begin_frame_source_ = nullptr;
  output_surface_ = nullptr;
  need_animate_input_ = false;
 }
