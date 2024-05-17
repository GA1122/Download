  ~FrameDeliverer() {
    DCHECK(io_thread_checker_.CalledOnValidThread());
    if (gpu_memory_buffer_pool_) {
      DropCurrentPoolTasks();
      media_task_runner_->DeleteSoon(FROM_HERE,
                                     gpu_memory_buffer_pool_.release());
    }
  }
