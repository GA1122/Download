void BaseAudioContext::HandlePostRenderTasks() {
  DCHECK(IsAudioThread());

  if (TryLock()) {
    GetDeferredTaskHandler().BreakConnections();

    GetDeferredTaskHandler().HandleDeferredTasks();
    GetDeferredTaskHandler().RequestToDeleteHandlersOnMainThread();

    unlock();
  }
}
