void BaseAudioContext::SetContextState(AudioContextState new_state) {
  DCHECK(IsMainThread());

  if (new_state == context_state_) {
    return;
  }

  switch (new_state) {
    case kSuspended:
      DCHECK_EQ(context_state_, kRunning);
      break;
    case kRunning:
      DCHECK_EQ(context_state_, kSuspended);
      break;
    case kClosed:
      DCHECK_NE(context_state_, kClosed);
      break;
  }

  context_state_ = new_state;

  if (was_audible_ && context_state_ != kRunning) {
    was_audible_ = false;
    GetExecutionContext()
        ->GetTaskRunner(TaskType::kMediaElementEvent)
        ->PostTask(FROM_HERE,
                   WTF::Bind(&BaseAudioContext::NotifyAudibleAudioStopped,
                             WrapPersistent(this)));
  }

  if (new_state == kClosed)
    GetDeferredTaskHandler().StopAcceptingTailProcessing();

  if (GetExecutionContext()) {
    GetExecutionContext()
        ->GetTaskRunner(TaskType::kMediaElementEvent)
        ->PostTask(FROM_HERE, WTF::Bind(&BaseAudioContext::NotifyStateChange,
                                        WrapPersistent(this)));
  }
}
