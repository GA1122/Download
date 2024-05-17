void OfflineAudioDestinationHandler::NotifyComplete() {
  DCHECK(IsMainThread());

  render_thread_.reset();

  if (IsExecutionContextDestroyed()) {
    return;
  }

  if (Context() && Context()->GetExecutionContext())
    Context()->FireCompletionEvent();
}
