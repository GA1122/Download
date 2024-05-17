void GpuProcessHost::OnAcceleratedSurfaceBuffersSwapped(
    const GpuHostMsg_AcceleratedSurfaceBuffersSwapped_Params& params) {
  TRACE_EVENT0("gpu", "GpuProcessHost::OnAcceleratedSurfaceBuffersSwapped");

  base::ScopedClosureRunner scoped_completion_runner(
      base::Bind(&AcceleratedSurfaceBuffersSwappedCompleted,
                 host_id_,
                 params.route_id,
                 true));

  gfx::PluginWindowHandle handle =
      GpuSurfaceTracker::Get()->GetSurfaceWindowHandle(params.surface_id);
  if (!handle)
    return;

  scoped_refptr<AcceleratedPresenter> presenter(
      AcceleratedPresenter::GetForWindow(handle));
  if (!presenter)
    return;

  scoped_completion_runner.Release();
  presenter->AsyncPresentAndAcknowledge(
      params.size,
      params.surface_handle,
      base::Bind(&AcceleratedSurfaceBuffersSwappedCompleted,
                 host_id_,
                 params.route_id));
}
