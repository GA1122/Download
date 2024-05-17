void RenderProcessHostImpl::SetCreateStoragePartitionServiceFunction(
    CreateStoragePartitionServiceFunction function) {
  g_create_storage_partition = function;
}
