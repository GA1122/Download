RenderWidgetHostViewPort* GetRenderWidgetHostViewFromSurfaceID(
    int surface_id) {
  int render_process_id = 0;
  int render_widget_id = 0;
  if (!GpuSurfaceTracker::Get()->GetRenderWidgetIDForSurface(
        surface_id, &render_process_id, &render_widget_id))
    return NULL;

  RenderProcessHost* process = RenderProcessHost::FromID(render_process_id);
  if (!process)
    return NULL;

  RenderWidgetHost* host = process->GetRenderWidgetHostByID(render_widget_id);
  return host ? RenderWidgetHostViewPort::FromRWHV(host->GetView()) : NULL;
}
