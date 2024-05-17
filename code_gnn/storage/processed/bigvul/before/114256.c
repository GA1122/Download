void GpuChannelHost::MessageFilter::OnChannelError() {
  DCHECK(parent_->factory_->IsIOThread());
  for (ListenerMap::iterator it = listeners_.begin();
       it != listeners_.end();
       it++) {
    const GpuListenerInfo& info = it->second;
    info.loop->PostTask(
        FROM_HERE,
        base::Bind(&IPC::Channel::Listener::OnChannelError, info.listener));
  }

  listeners_.clear();

  MessageLoop* main_loop = parent_->factory_->GetMainLoop();
  main_loop->PostTask(FROM_HERE,
                      base::Bind(&GpuChannelHost::OnChannelError, parent_));
 }
