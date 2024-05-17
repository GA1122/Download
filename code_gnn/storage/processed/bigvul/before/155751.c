  identity::mojom::IdentityManager& GetIdentityManager() {
    DCHECK_CALLED_ON_VALID_SEQUENCE(host_->sequence_checker_);
    if (!identity_manager_) {
      host_->delegate_->GetConnector()->BindInterface(
          identity::mojom::kServiceName, mojo::MakeRequest(&identity_manager_));
    }
    return *identity_manager_;
  }
