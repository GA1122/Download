bool Bluetooth::IsScanActive(mojo::BindingId id) const {
  return client_bindings_.HasBinding(id);
}
