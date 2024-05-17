void ResourceMessageFilter::OnEstablishGpuChannel() {
  GpuProcessHost::Get()->EstablishGpuChannel(id(), this);
}
