  virtual void CleanUp() {
    delete gpu_process_;
    if (child_thread_)
      delete child_thread_;
  }
