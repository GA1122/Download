void BindDiscardableMemoryRequestOnIO(
    discardable_memory::mojom::DiscardableSharedMemoryManagerRequest request,
    discardable_memory::DiscardableSharedMemoryManager* manager) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  service_manager::BindSourceInfo source_info;
  manager->Bind(std::move(request), source_info);
}
