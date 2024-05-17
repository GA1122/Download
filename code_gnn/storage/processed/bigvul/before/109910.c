void GpuProcessHost::OnAcceleratedSurfaceSuspend(int32 surface_id) {
  TRACE_EVENT0("gpu", "GpuProcessHost::OnAcceleratedSurfaceSuspend");

  gfx::PluginWindowHandle handle =
      GpuSurfaceTracker::Get()->GetSurfaceWindowHandle(surface_id);
  if (!handle)
    return;

  scoped_refptr<AcceleratedPresenter> presenter(
      AcceleratedPresenter::GetForWindow(handle));
  if (!presenter)
    return;

  presenter->Suspend();
}
