void GpuProcessHost::OnImageCreated(const gfx::Size size) {
  TRACE_EVENT0("gpu", "GpuProcessHost::OnImageCreated");

  if (!create_image_requests_.empty()) {
    CreateImageCallback callback = create_image_requests_.front();
    create_image_requests_.pop();
    callback.Run(size);
  }
}
