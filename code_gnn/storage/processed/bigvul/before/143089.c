void OfflineAudioDestinationHandler::NotifySuspend(size_t frame) {
  DCHECK(IsMainThread());

  if (Context() && Context()->GetExecutionContext())
    Context()->ResolveSuspendOnMainThread(frame);
}
