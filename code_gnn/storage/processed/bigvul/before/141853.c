 void KioskNextHomeInterfaceBrokerImpl::BindRequest(
    mojom::KioskNextHomeInterfaceBrokerRequest request) {
  bindings_.AddBinding(this, std::move(request));
}
