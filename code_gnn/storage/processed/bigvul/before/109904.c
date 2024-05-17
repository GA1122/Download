  virtual void Init() {
    if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kSingleProcess)) {
      child_thread_ = new GpuChildThread(channel_id_);
    } else {
      gpu_process_ = new GpuProcess();
      gpu_process_->set_main_thread(new GpuChildThread(channel_id_));
    }
  }
