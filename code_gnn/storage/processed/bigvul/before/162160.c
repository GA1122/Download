void RenderProcessHost::WarmupSpareRenderProcessHost(
    content::BrowserContext* browser_context) {
  g_spare_render_process_host_manager.Get().WarmupSpareRenderProcessHost(
      browser_context);
}
