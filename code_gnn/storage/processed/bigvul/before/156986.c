  void OnVideoFrame(scoped_refptr<media::VideoFrame> frame) {
    DCHECK(io_thread_checker_.CalledOnValidThread());

#if defined(OS_ANDROID)
    if (render_frame_suspended_)
      return;
#endif   

    if (!gpu_memory_buffer_pool_) {
      EnqueueFrame(std::move(frame));
      return;
    }

#if defined(OS_WIN)
    const bool skip_creating_gpu_memory_buffer =
        frame->visible_rect().width() <
            kUseGpuMemoryBufferVideoFramesMinResolution.width() ||
        frame->visible_rect().height() <
            kUseGpuMemoryBufferVideoFramesMinResolution.height();
#else
    const bool skip_creating_gpu_memory_buffer = false;
#endif   

    if (render_frame_suspended_ || skip_creating_gpu_memory_buffer) {
      EnqueueFrame(std::move(frame));
      DropCurrentPoolTasks();
      return;
    }

    media_task_runner_->PostTask(
        FROM_HERE,
        base::BindOnce(
            &media::GpuMemoryBufferVideoFramePool::MaybeCreateHardwareFrame,
            base::Unretained(gpu_memory_buffer_pool_.get()), frame,
            media::BindToCurrentLoop(
                base::BindOnce(&FrameDeliverer::EnqueueFrame,
                               weak_factory_for_pool_.GetWeakPtr()))));
  }
