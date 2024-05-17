void ResourceMessageFilter::OnSynchronizeGpu(IPC::Message* reply) {
  GpuProcessHost::Get()->Synchronize(reply, this);
}
