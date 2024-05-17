ContentGpuClient* ShellMainDelegate::CreateContentGpuClient() {
  gpu_client_.reset(new ShellContentGpuClient);
  return gpu_client_.get();
}
