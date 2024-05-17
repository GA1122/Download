BrowserChildProcessHostImpl::TakeInProcessServiceRequest() {
  DCHECK(broker_client_invitation_);
  auto invitation = std::move(broker_client_invitation_);
  return service_manager::mojom::ServiceRequest(
      invitation->ExtractInProcessMessagePipe(
          child_connection_->service_token()));
}
