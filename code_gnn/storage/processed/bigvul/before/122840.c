  virtual ~BrowserCompositorOutputSurface() {
    DCHECK(CalledOnValidThread());
    if (!client_)
      return;
    output_surface_proxy_->RemoveSurface(surface_id_);
  }
