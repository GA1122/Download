void RenderProcessHostImpl::CreateStoragePartitionService(
    mojom::StoragePartitionServiceRequest request) {
  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableMojoLocalStorage)) {
    if (g_create_storage_partition) {
      g_create_storage_partition(this, std::move(request));
      return;
    }

    storage_partition_impl_->Bind(id_, std::move(request));
  }
}
