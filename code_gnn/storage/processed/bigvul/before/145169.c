void GpuProcessHost::OnDestroyingVideoSurfaceAck() {
  TRACE_EVENT0("gpu", "GpuProcessHost::OnDestroyingVideoSurfaceAck");
  if (!send_destroying_video_surface_done_cb_.is_null())
    base::ResetAndReturn(&send_destroying_video_surface_done_cb_).Run();
}
