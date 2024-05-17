mojo::BindingId StoragePartitionImpl::Bind(
    int process_id,
    mojo::InterfaceRequest<blink::mojom::StoragePartitionService> request) {
  return bindings_.AddBinding(this, std::move(request), process_id);
}
