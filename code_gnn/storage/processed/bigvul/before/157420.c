void BaseAudioContext::HandlePreRenderTasks(
    const AudioIOPosition& output_position) {
  DCHECK(IsAudioThread());

  if (TryLock()) {
    GetDeferredTaskHandler().HandleDeferredTasks();

    ResolvePromisesForUnpause();

    HandleStoppableSourceNodes();

    listener()->UpdateState();

    output_position_ = output_position;

    unlock();
  }
}
