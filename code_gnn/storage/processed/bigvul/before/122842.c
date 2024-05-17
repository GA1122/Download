  virtual ~GpuProcessTransportFactory() {
    DCHECK(per_compositor_data_.empty());
  }
