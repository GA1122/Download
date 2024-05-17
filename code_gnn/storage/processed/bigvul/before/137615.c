WebContents* GetWebContents(int render_process_id, int render_view_id) {
  content::RenderViewHost* render_view_host =
      content::RenderViewHost::FromID(render_process_id, render_view_id);

  if (!render_view_host)
    return nullptr;

  return WebContents::FromRenderViewHost(render_view_host);
}
