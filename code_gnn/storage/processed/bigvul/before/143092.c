void OfflineAudioDestinationHandler::PrepareTaskRunnerForRendering() {
  DCHECK(IsMainThread());

  AudioWorklet* audio_worklet = Context()->audioWorklet();
  if (audio_worklet && audio_worklet->IsReady()) {
    if (!render_thread_) {
      if (!render_thread_task_runner_) {
        render_thread_task_runner_ =
            audio_worklet->GetMessagingProxy()->GetBackingWorkerThread()
                         ->GetTaskRunner(TaskType::kMiscPlatformAPI);
      }
    } else {
      render_thread_ = nullptr;
      render_thread_task_runner_ =
          audio_worklet->GetMessagingProxy()->GetBackingWorkerThread()
                       ->GetTaskRunner(TaskType::kMiscPlatformAPI);
    }
  } else {
    if (!render_thread_) {
      render_thread_ = Platform::Current()->CreateThread(
          ThreadCreationParams(WebThreadType::kOfflineAudioRenderThread));
      render_thread_task_runner_ = render_thread_->GetTaskRunner();
    }
  }

  DCHECK(render_thread_task_runner_);
}
