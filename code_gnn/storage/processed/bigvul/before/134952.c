identity::mojom::IdentityManager& DriveFsHost::GetIdentityManager() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (!identity_manager_) {
    delegate_->GetConnector()->BindInterface(
        identity::mojom::kServiceName, mojo::MakeRequest(&identity_manager_));
  }
  return *identity_manager_;
}
