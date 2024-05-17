void RenderProcessHostImpl::CleanupSpareRenderProcessHost() {
  g_spare_render_process_host_manager.Get().CleanupSpareRenderProcessHost();
}
