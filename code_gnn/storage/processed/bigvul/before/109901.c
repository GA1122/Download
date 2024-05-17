  explicit GpuMainThread(const std::string& channel_id)
      : base::Thread("Chrome_InProcGpuThread"),
        channel_id_(channel_id),
        gpu_process_(NULL),
        child_thread_(NULL) {
  }
