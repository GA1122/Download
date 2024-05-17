void GpuProcessHost::DeleteImage(
    int client_id,
    int image_id,
    int sync_point) {
  TRACE_EVENT0("gpu", "GpuProcessHostUIShim::DeleteImage");

  DCHECK(CalledOnValidThread());

  Send(new GpuMsg_DeleteImage(client_id, image_id, sync_point));
}
