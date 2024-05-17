void GpuProcessHostUIShim::OnUpdateVSyncParameters(int surface_id,
                                             base::TimeTicks timebase,
                                             base::TimeDelta interval) {

  int render_process_id = 0;
  int render_widget_id = 0;
  if (!GpuSurfaceTracker::Get()->GetRenderWidgetIDForSurface(
      surface_id, &render_process_id, &render_widget_id)) {
    return;
  }
  RenderProcessHost* host = RenderProcessHost::FromID(render_process_id);
  if (!host)
    return;
  RenderWidgetHost* rwh = host->GetRenderWidgetHostByID(render_widget_id);
  if (!rwh)
    return;
  RenderWidgetHostImpl::From(rwh)->UpdateVSyncParameters(timebase, interval);
}
