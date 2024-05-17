void SynchronousCompositor::SetGpuService(
    scoped_refptr<gpu::InProcessCommandBuffer::Service> service) {
  g_factory.Get().SetDeferredGpuService(service);
  GpuProcessHost::RegisterGpuMainThreadFactory(
      CreateInProcessGpuThreadForSynchronousCompositor);
}
