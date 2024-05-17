void RenderProcessHostImpl::InitializeChannelProxy() {
  scoped_refptr<base::SingleThreadTaskRunner> io_task_runner =
      BrowserThread::GetTaskRunnerForThread(BrowserThread::IO);

  service_manager::Connector* connector =
      BrowserContext::GetConnectorFor(browser_context_);
  if (!connector) {
    if (!ServiceManagerConnection::GetForProcess()) {
      ServiceManagerConnection::SetForProcess(ServiceManagerConnection::Create(
          mojo::MakeRequest(&test_service_), io_task_runner));
    }
    connector = ServiceManagerConnection::GetForProcess()->GetConnector();
  }

  broker_client_invitation_ =
      std::make_unique<mojo::edk::OutgoingBrokerClientInvitation>();
  service_manager::Identity child_identity(
      mojom::kRendererServiceName,
      BrowserContext::GetServiceUserIdFor(GetBrowserContext()),
      base::StringPrintf("%d_%d", id_, instance_id_++));
  child_connection_.reset(new ChildConnection(child_identity,
                                              broker_client_invitation_.get(),
                                              connector, io_task_runner));

  mojo::MessagePipe pipe;
  BindInterface(IPC::mojom::ChannelBootstrap::Name_, std::move(pipe.handle1));
  std::unique_ptr<IPC::ChannelFactory> channel_factory =
      IPC::ChannelMojo::CreateServerFactory(
          std::move(pipe.handle0), io_task_runner,
          base::ThreadTaskRunnerHandle::Get());

  content::BindInterface(this, &child_control_interface_);

  ResetChannelProxy();

#if defined(OS_ANDROID)
  if (GetContentClient()->UsingSynchronousCompositing()) {
    channel_ = IPC::SyncChannel::Create(this, io_task_runner.get(),
                                        base::ThreadTaskRunnerHandle::Get(),
                                        &never_signaled_);
  }
#endif   
  if (!channel_)
    channel_.reset(new IPC::ChannelProxy(this, io_task_runner.get(),
                                         base::ThreadTaskRunnerHandle::Get()));
  channel_->Init(std::move(channel_factory), true  );

  channel_->GetRemoteAssociatedInterface(&remote_route_provider_);
  channel_->GetRemoteAssociatedInterface(&renderer_interface_);

  channel_->Pause();
}
