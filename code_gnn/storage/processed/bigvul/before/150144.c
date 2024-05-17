  void SetCallback(LayerTreeHostImpl* host_impl, bool enable) {
    host_impl->SetTreeActivationCallback(
        enable ? base::Bind(
                     &LayerTreeHostTestKeepSwapPromiseMFBA::ActivationCallback,
                     base::Unretained(this))
               : base::Closure());
  }
