void AudioHandler::BreakConnection() {
  bool has_lock = false;
  if (Context()->IsAudioThread()) {
    has_lock = Context()->TryLock();
  } else {
    Context()->lock();
    has_lock = true;
  }

  if (has_lock) {
    BreakConnectionWithLock();
    Context()->unlock();
  } else {
    DCHECK(Context()->IsAudioThread());
    Context()->GetDeferredTaskHandler().AddDeferredBreakConnection(*this);
  }
}
