RenderProcessHost* RenderProcessHostImpl::CreateOrUseSpareRenderProcessHost(
    BrowserContext* browser_context,
    StoragePartitionImpl* storage_partition_impl,
    SiteInstance* site_instance,
    bool is_for_guests_only) {
  RenderProcessHost* render_process_host =
      g_spare_render_process_host_manager.Get().MaybeTakeSpareRenderProcessHost(
          browser_context, storage_partition_impl, site_instance,
          is_for_guests_only);

  if (!render_process_host) {
    render_process_host =
        CreateRenderProcessHost(browser_context, storage_partition_impl,
                                site_instance, is_for_guests_only);
  }

  DCHECK(render_process_host);
  return render_process_host;
}
