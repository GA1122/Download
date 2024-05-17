void BaseAudioContext::HandlePreRenderTasks(
    const AudioIOPosition& output_position,
    const AudioIOCallbackMetric& metric) {
  DCHECK(IsAudioThread());

  if (TryLock()) {
    GetDeferredTaskHandler().HandleDeferredTasks();

    ResolvePromisesForUnpause();

    HandleStoppableSourceNodes();

    listener()->UpdateState();

    output_position_ = output_position;
    callback_metric_ = metric;

    unlock();
  }
}
