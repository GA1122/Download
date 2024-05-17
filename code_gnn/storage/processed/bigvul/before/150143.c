  void SetCallback(LayerTreeHostImpl* host_impl, bool enable) {
    host_impl->SetTreeActivationCallback(
        enable
            ? base::Bind(&LayerTreeHostTestKeepSwapPromise::ActivationCallback,
                         base::Unretained(this))
            : base::Closure());
  }
