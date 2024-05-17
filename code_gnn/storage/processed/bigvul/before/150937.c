void Bluetooth::CancelScan(mojo::BindingId id) {
  client_bindings_.RemoveBinding(id);
}
