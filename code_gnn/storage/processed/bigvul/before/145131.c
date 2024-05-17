void GpuProcessHost::BindInterface(
    const std::string& interface_name,
    mojo::ScopedMessagePipeHandle interface_pipe) {
  if (interface_name ==
      discardable_memory::mojom::DiscardableSharedMemoryManager::Name_) {
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(
            &BindDiscardableMemoryRequestOnUI,
            discardable_memory::mojom::DiscardableSharedMemoryManagerRequest(
                std::move(interface_pipe))));
    return;
  }
  process_->child_connection()->BindInterface(interface_name,
                                              std::move(interface_pipe));
}
