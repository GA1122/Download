void ServiceManagerConnectionImpl::Start() {
  context_->Start(
      base::Bind(&ServiceManagerConnectionImpl::OnConnectionLost,
                 weak_factory_.GetWeakPtr()));
}
