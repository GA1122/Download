  void WarmupSpareRenderProcessHost(BrowserContext* browser_context) {
    StoragePartitionImpl* current_partition =
        static_cast<StoragePartitionImpl*>(
            BrowserContext::GetStoragePartition(browser_context, nullptr));

    if (spare_render_process_host_ &&
        matching_browser_context_ == browser_context &&
        matching_storage_partition_ == current_partition)
      return;   

    CleanupSpareRenderProcessHost();

    if (RenderProcessHost::run_renderer_in_process() ||
        g_all_hosts.Get().size() >=
            RenderProcessHostImpl::GetMaxRendererProcessCount())
      return;

    matching_browser_context_ = browser_context;
    matching_storage_partition_ = current_partition;

    spare_render_process_host_ = RenderProcessHostImpl::CreateRenderProcessHost(
        browser_context, current_partition, nullptr,
        false  );
    spare_render_process_host_->AddObserver(this);
    spare_render_process_host_->Init();
  }
