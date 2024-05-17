  void SetCallback(LayerTreeHostImpl* host_impl, bool enable) {
    host_impl->SetTreeActivationCallback(
        enable
            ? base::Bind(
                  &LayerTreeHostTestTreeActivationCallback::ActivationCallback,
                  base::Unretained(this))
            : base::Closure());
  }
