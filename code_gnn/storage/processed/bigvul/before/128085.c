void SynchronousCompositorImpl::RegisterWithClient() {
  DCHECK(CalledOnValidThread());
  DCHECK(output_surface_);
  DCHECK(synchronous_input_handler_proxy_);
  DCHECK(!registered_with_client_);
  registered_with_client_ = true;

  compositor_client_->DidInitializeCompositor(this);

  output_surface_->SetTreeActivationCallback(
    base::Bind(&SynchronousCompositorImpl::DidActivatePendingTree,
               weak_ptr_factory_.GetWeakPtr()));

  synchronous_input_handler_proxy_->SetOnlySynchronouslyAnimateRootFlings(this);
}
