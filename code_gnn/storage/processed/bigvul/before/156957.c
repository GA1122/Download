  FrameDeliverer(const base::WeakPtr<WebMediaPlayerMS>& player,
                 const MediaStreamVideoRenderer::RepaintCB& enqueue_frame_cb,
                 scoped_refptr<base::SingleThreadTaskRunner> media_task_runner,
                 scoped_refptr<base::TaskRunner> worker_task_runner,
                 media::GpuVideoAcceleratorFactories* gpu_factories)
      : last_frame_opaque_(true),
        last_frame_rotation_(media::VIDEO_ROTATION_0),
        received_first_frame_(false),
        main_task_runner_(base::ThreadTaskRunnerHandle::Get()),
        player_(player),
        enqueue_frame_cb_(enqueue_frame_cb),
        media_task_runner_(media_task_runner),
        weak_factory_for_pool_(this),
        weak_factory_(this) {
    io_thread_checker_.DetachFromThread();

    if (gpu_factories && gpu_factories->ShouldUseGpuMemoryBuffersForVideoFrames(
                             true  )) {
      gpu_memory_buffer_pool_.reset(new media::GpuMemoryBufferVideoFramePool(
          media_task_runner, worker_task_runner, gpu_factories));
    }
  }
