void GpuProcessHost::CreateImageError(
    const CreateImageCallback& callback, const gfx::Size size) {
  callback.Run(size);
}
