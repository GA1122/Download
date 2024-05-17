void BaseAudioContext::HandlePostRenderTasks(const AudioBus* destination_bus) {
  DCHECK(IsAudioThread());

  if (TryLock()) {
    GetDeferredTaskHandler().BreakConnections();

    GetDeferredTaskHandler().HandleDeferredTasks();
    GetDeferredTaskHandler().RequestToDeleteHandlersOnMainThread();

    unlock();
  }

  if (HasRealtimeConstraint()) {
    bool is_audible = IsAudible(destination_bus);

    if (is_audible) {
      ++total_audible_renders_;
    }

    if (was_audible_ != is_audible) {
      was_audible_ = is_audible;
      if (is_audible) {
        PostCrossThreadTask(
            *task_runner_, FROM_HERE,
            CrossThreadBind(&BaseAudioContext::NotifyAudibleAudioStarted,
                            WrapCrossThreadPersistent(this)));
      } else {
        PostCrossThreadTask(
            *task_runner_, FROM_HERE,
            CrossThreadBind(&BaseAudioContext::NotifyAudibleAudioStopped,
                            WrapCrossThreadPersistent(this)));
      }
    }
  }
}
