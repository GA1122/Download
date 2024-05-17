  void GetInterface(const std::string& interface_name,
                    mojo::ScopedMessagePipeHandle handle) {
    DCHECK(thread_checker_.CalledOnValidThread());
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    service_manager::mojom::InterfaceProvider* provider = registry_.get();

    base::AutoLock lock(enabled_lock_);
    if (enabled_)
      provider->GetInterface(interface_name, std::move(handle));
  }
