  MockMediaStreamDispatcherHost(int render_process_id,
                                int render_frame_id,
                                MediaStreamManager* manager)
      : MediaStreamDispatcherHost(render_process_id, render_frame_id, manager),
        task_runner_(base::ThreadTaskRunnerHandle::Get()),
        binding_(this) {}
