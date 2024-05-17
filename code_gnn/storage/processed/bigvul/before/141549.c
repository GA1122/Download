void TracingControllerImpl::ConnectToServiceIfNeeded() {
  if (!coordinator_) {
    ServiceManagerConnection::GetForProcess()->GetConnector()->BindInterface(
        tracing::mojom::kServiceName, &coordinator_);
  }
}
