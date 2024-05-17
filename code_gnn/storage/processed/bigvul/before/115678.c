void HostNPScriptObject::DisconnectInternal() {
  if (MessageLoop::current() != host_context_.main_message_loop()) {
    host_context_.main_message_loop()->PostTask(
        FROM_HERE, base::Bind(&HostNPScriptObject::DisconnectInternal,
                              base::Unretained(this)));
    return;
  }

  switch (state_) {
    case kDisconnected:
      disconnected_event_.Signal();
      return;

    case kStarting:
      SetState(kDisconnecting);
      SetState(kDisconnected);
      disconnected_event_.Signal();
      return;

    case kDisconnecting:
      return;

    default:
      DCHECK(host_);
      SetState(kDisconnecting);
      host_->Shutdown(
          base::Bind(&HostNPScriptObject::OnShutdownFinished,
                     base::Unretained(this)));
  }
 }
