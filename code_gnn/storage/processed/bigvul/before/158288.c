device::mojom::WakeLock* RenderWidgetHostImpl::GetWakeLock() {
  if (!wake_lock_) {
    device::mojom::WakeLockRequest request = mojo::MakeRequest(&wake_lock_);
    if (ServiceManagerConnection::GetForProcess()) {
      service_manager::Connector* connector =
          ServiceManagerConnection::GetForProcess()->GetConnector();
      DCHECK(connector);
      device::mojom::WakeLockProviderPtr wake_lock_provider;
      connector->BindInterface(device::mojom::kServiceName,
                               mojo::MakeRequest(&wake_lock_provider));
      wake_lock_provider->GetWakeLockWithoutContext(
          device::mojom::WakeLockType::kPreventDisplaySleep,
          device::mojom::WakeLockReason::kOther, "GetSnapshot",
          std::move(request));
    }
  }
  return wake_lock_.get();
}
