bool WtsSessionProcessDelegate::Core::Send(IPC::Message* message) {
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  if (channel_.get()) {
    return channel_->Send(message);
  } else {
    delete message;
    return false;
   }
 }
