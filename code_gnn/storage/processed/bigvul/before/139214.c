bool RenderProcessHostImpl::Send(IPC::Message* msg) {
  TRACE_EVENT0("renderer_host", "RenderProcessHostImpl::Send");

  std::unique_ptr<IPC::Message> message(msg);

  if (!channel_)
    return false;

#if !defined(OS_ANDROID)
  DCHECK(!message->is_sync());
#else
  if (message->is_sync()) {
    if (!HasConnection())
      return false;

    if (child_process_launcher_.get() && child_process_launcher_->IsStarting())
      return false;
  }
#endif

  return channel_->Send(message.release());
}
