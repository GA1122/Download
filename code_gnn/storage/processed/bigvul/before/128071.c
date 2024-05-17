base::Thread* CreateInProcessGpuThreadForSynchronousCompositor(
    const InProcessChildThreadParams& params) {
  return g_factory.Get().CreateInProcessGpuThread(params);
}
