void GpuProcessHost::OnAcceleratedSurfaceSuspend(int32 surface_id) {
  TRACE_EVENT0("renderer",
      "GpuProcessHost::OnAcceleratedSurfaceSuspend");

  gfx::GLSurfaceHandle handle = GpuSurfaceTracker::Get()->GetSurfaceHandle(
      surface_id);
  if (!handle.handle)
    return;

  scoped_refptr<AcceleratedPresenter> presenter(
      AcceleratedPresenter::GetForWindow(handle.handle));
  if (!presenter)
    return;

  presenter->Suspend();
}
