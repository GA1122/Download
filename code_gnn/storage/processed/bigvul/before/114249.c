void GpuChannelHost::AddRoute(
    int route_id, base::WeakPtr<IPC::Channel::Listener> listener) {
  DCHECK(MessageLoopProxy::current());

  scoped_refptr<base::MessageLoopProxy> io_loop = factory_->GetIOLoopProxy();
  io_loop->PostTask(FROM_HERE,
                    base::Bind(&GpuChannelHost::MessageFilter::AddRoute,
                               channel_filter_.get(), route_id, listener,
                               MessageLoopProxy::current()));
}
