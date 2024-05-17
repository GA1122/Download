BrowserChildProcessHostImpl::BrowserChildProcessHostImpl(
    content::ProcessType process_type,
    BrowserChildProcessHostDelegate* delegate,
    const std::string& service_name)
    : data_(process_type),
      delegate_(delegate),
      broker_client_invitation_(new mojo::edk::OutgoingBrokerClientInvitation),
      channel_(nullptr),
      is_channel_connected_(false),
      notify_child_disconnected_(false),
      weak_factory_(this) {
  data_.id = ChildProcessHostImpl::GenerateChildProcessUniqueId();

  child_process_host_.reset(ChildProcessHost::Create(this));
  AddFilter(new TraceMessageFilter(data_.id));

  g_child_process_list.Get().push_back(this);
  GetContentClient()->browser()->BrowserChildProcessHostCreated(this);

  if (!service_name.empty()) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    service_manager::Identity child_identity(
        service_name, service_manager::mojom::kInheritUserID,
        base::StringPrintf("%d", data_.id));
    child_connection_.reset(
        new ChildConnection(child_identity, broker_client_invitation_.get(),
                            ServiceManagerContext::GetConnectorForIOThread(),
                            base::ThreadTaskRunnerHandle::Get()));
  }

  CreateMetricsAllocator();
}
