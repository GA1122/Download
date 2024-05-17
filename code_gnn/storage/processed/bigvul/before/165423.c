void StoragePartitionImpl::OpenSessionStorage(
    const std::string& namespace_id,
    blink::mojom::SessionStorageNamespaceRequest request) {
  int process_id = bindings_.dispatch_context();
  dom_storage_context_->OpenSessionStorage(process_id, namespace_id,
                                           bindings_.GetBadMessageCallback(),
                                           std::move(request));
}
