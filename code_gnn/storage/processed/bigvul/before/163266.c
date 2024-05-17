  explicit GpuDataManagerVisualProxy(GpuDataManagerImpl* gpu_data_manager)
      : gpu_data_manager_(gpu_data_manager) {
    gpu_data_manager_->AddObserver(this);
  }
