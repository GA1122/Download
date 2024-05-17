  RenderProcessHost* MaybeTakeSpareRenderProcessHost(
      BrowserContext* browser_context,
      StoragePartition* partition,
      SiteInstance* site_instance,
      bool is_for_guests_only) {
    if (spare_render_process_host_ &&
        browser_context == matching_browser_context_ && !is_for_guests_only &&
        !partition) {
      partition =
          BrowserContext::GetStoragePartition(browser_context, site_instance);
    }

    if (!spare_render_process_host_ ||
        browser_context != matching_browser_context_ ||
        partition != matching_storage_partition_ || is_for_guests_only) {
      CleanupSpareRenderProcessHost();
      return nullptr;
    }

    CHECK(spare_render_process_host_->HostHasNotBeenUsed());
    RenderProcessHost* rph = spare_render_process_host_;
    DropSpareRenderProcessHost(spare_render_process_host_);
    return rph;
  }
