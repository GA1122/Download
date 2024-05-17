void RenderProcessHostImpl::InitializeChannelProxy() {
  scoped_refptr<base::SingleThreadTaskRunner> io_task_runner =
      BrowserThread::GetTaskRunnerForThread(BrowserThread::IO);

  service_manager::Connector* connector =
      BrowserContext::GetConnectorFor(browser_context_);
  if (!connector) {
    if (!ServiceManagerConnection::GetForProcess()) {
      service_manager::mojom::ServiceRequest request(&test_service_);
      ServiceManagerConnection::SetForProcess(ServiceManagerConnection::Create(
          std::move(request), io_task_runner));
    }
    connector = ServiceManagerConnection::GetForProcess()->GetConnector();
  }

  pending_connection_.reset(new mojo::edk::PendingProcessConnection);
  child_connection_.reset(new ChildConnection(
      mojom::kRendererServiceName,
      base::StringPrintf("%d_%d", id_, instance_id_++),
      pending_connection_.get(), connector, io_task_runner));

  IPC::mojom::ChannelBootstrapPtr bootstrap;
  GetRemoteInterfaces()->GetInterface(&bootstrap);
  std::unique_ptr<IPC::ChannelFactory> channel_factory =
      IPC::ChannelMojo::CreateServerFactory(
          bootstrap.PassInterface().PassHandle(), io_task_runner);

  ResetChannelProxy();

#if defined(OS_ANDROID)
  if (GetContentClient()->UsingSynchronousCompositing()) {
    channel_ = IPC::SyncChannel::Create(
        this, io_task_runner.get(), &never_signaled_);
  }
#endif   
  if (!channel_)
    channel_.reset(new IPC::ChannelProxy(this, io_task_runner.get()));
  channel_->Init(std::move(channel_factory), true  );

  channel_->GetRemoteAssociatedInterface(&remote_route_provider_);
  channel_->GetRemoteAssociatedInterface(&renderer_interface_);

  channel_->Pause();
}
