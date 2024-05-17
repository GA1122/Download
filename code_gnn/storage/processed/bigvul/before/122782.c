void AcceleratedSurfaceBuffersSwappedCompletedForRenderer(
    int surface_id,
    base::TimeTicks timebase,
    base::TimeDelta interval) {
  if (!BrowserThread::CurrentlyOn(BrowserThread::UI)) {
    BrowserThread::PostTask(
        BrowserThread::UI,
        FROM_HERE,
        base::Bind(&AcceleratedSurfaceBuffersSwappedCompletedForRenderer,
                   surface_id, timebase, interval));
    return;
  }

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
  RenderWidgetHostImpl::From(rwh)->AcknowledgeSwapBuffersToRenderer();
  if (interval != base::TimeDelta())
    RenderWidgetHostImpl::From(rwh)->UpdateVSyncParameters(timebase, interval);
}
