void GpuProcessHost::OnAcceleratedSurfaceRelease(
    const GpuHostMsg_AcceleratedSurfaceRelease_Params& params) {
  TRACE_EVENT0("renderer",
      "GpuProcessHost::OnAcceleratedSurfaceRelease");

  gfx::GLSurfaceHandle handle = GpuSurfaceTracker::Get()->GetSurfaceHandle(
      params.surface_id);
  if (!handle.handle)
    return;

  scoped_refptr<AcceleratedPresenter> presenter(
      AcceleratedPresenter::GetForWindow(handle.handle));
  if (!presenter)
    return;

  presenter->ReleaseSurface();
}
