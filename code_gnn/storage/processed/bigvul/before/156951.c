  void DropCurrentPoolTasks() {
    DCHECK(io_thread_checker_.CalledOnValidThread());
    DCHECK(gpu_memory_buffer_pool_);

    if (!weak_factory_for_pool_.HasWeakPtrs())
      return;

    media_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(&media::GpuMemoryBufferVideoFramePool::Abort,
                       base::Unretained(gpu_memory_buffer_pool_.get())));
    weak_factory_for_pool_.InvalidateWeakPtrs();
  }
