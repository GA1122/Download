void GpuMessageFilter::OnEstablishGpuChannel(
    content::CauseForGpuLaunch cause_for_gpu_launch,
    IPC::Message* reply) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  GpuProcessHost* host = GpuProcessHost::FromID(gpu_process_id_);
  if (!host) {
    host = GpuProcessHost::Get(GpuProcessHost::GPU_PROCESS_KIND_SANDBOXED,
                               cause_for_gpu_launch);
    if (!host) {
      reply->set_reply_error();
      Send(reply);
      return;
    }

    gpu_process_id_ = host->host_id();
  }

  host->EstablishGpuChannel(
      render_process_id_,
      share_contexts_,
      base::Bind(&GpuMessageFilter::EstablishChannelCallback,
                 AsWeakPtr(),
                 reply));
}
