CommandBufferProxyImpl::~CommandBufferProxyImpl() {
  for (auto& observer : deletion_observers_)
    observer.OnWillDeleteImpl();
  DisconnectChannel();
}
