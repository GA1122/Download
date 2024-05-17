void OfflineAudioDestinationHandler::FinishOfflineRendering() {
  DCHECK(!IsMainThread());

  PostCrossThreadTask(
      *main_thread_task_runner_, FROM_HERE,
      CrossThreadBind(&OfflineAudioDestinationHandler::NotifyComplete,
                      WrapRefCounted(this)));
}
