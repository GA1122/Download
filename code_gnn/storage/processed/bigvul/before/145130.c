void BindDiscardableMemoryRequestOnUI(
    discardable_memory::mojom::DiscardableSharedMemoryManagerRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

#if defined(USE_AURA)
  if (!features::IsAshInBrowserProcess()) {
    ServiceManagerConnection::GetForProcess()->GetConnector()->BindInterface(
        ui::mojom::kServiceName, std::move(request));
    return;
  }
#endif
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::BindOnce(
          &BindDiscardableMemoryRequestOnIO, std::move(request),
          BrowserMainLoop::GetInstance()->discardable_shared_memory_manager()));
}
