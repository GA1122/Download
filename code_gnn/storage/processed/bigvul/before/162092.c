  RenderProcessHost* GetProcessHost(SiteInstance* site_instance,
                                    bool is_for_guests_only) {
    StoragePartitionImpl* default_partition =
        static_cast<StoragePartitionImpl*>(
            BrowserContext::GetDefaultStoragePartition(browser_context_));
    StoragePartitionImpl* partition = static_cast<StoragePartitionImpl*>(
        BrowserContext::GetStoragePartition(browser_context_, site_instance));

    if (partition != default_partition || is_for_guests_only) {
      RenderProcessHost* host = RenderProcessHostImpl::CreateRenderProcessHost(
          browser_context_, partition, site_instance, is_for_guests_only);
      host->SetIsNeverSuitableForReuse();
      return host;
    }

    if (host_)
      return host_;

    host_ = RenderProcessHostImpl::CreateOrUseSpareRenderProcessHost(
        browser_context_, partition, site_instance,
        false  );
    host_->SetIsNeverSuitableForReuse();
    host_->AddObserver(this);

    return host_;
  }
