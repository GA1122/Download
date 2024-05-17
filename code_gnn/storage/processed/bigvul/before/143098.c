void OfflineAudioDestinationHandler::SuspendOfflineRendering() {
  DCHECK(!IsMainThread());

  PostCrossThreadTask(
      *main_thread_task_runner_, FROM_HERE,
      CrossThreadBind(&OfflineAudioDestinationHandler::NotifySuspend,
                      WrapRefCounted(this), Context()->CurrentSampleFrame()));
}
