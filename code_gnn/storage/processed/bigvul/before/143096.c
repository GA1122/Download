void OfflineAudioDestinationHandler::StartRendering() {
  DCHECK(IsMainThread());
  DCHECK(render_target_);
  DCHECK(render_thread_task_runner_);

  if (!is_rendering_started_) {
    is_rendering_started_ = true;
    PostCrossThreadTask(
        *render_thread_task_runner_, FROM_HERE,
        CrossThreadBind(&OfflineAudioDestinationHandler::StartOfflineRendering,
                        WrapRefCounted(this)));
    return;
  }

  PostCrossThreadTask(
      *render_thread_task_runner_, FROM_HERE,
      CrossThreadBind(&OfflineAudioDestinationHandler::DoOfflineRendering,
                      WrapRefCounted(this)));
}
