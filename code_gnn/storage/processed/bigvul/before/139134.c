void RenderProcessHostImpl::CreateStoragePartitionService(
    mojo::InterfaceRequest<mojom::StoragePartitionService> request) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kMojoLocalStorage)) {
    storage_partition_impl_->Bind(std::move(request));
  }
}
