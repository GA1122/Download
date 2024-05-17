GLuint GLES2Implementation::CreateClientGpuFenceCHROMIUM(
    ClientGpuFence source) {
  GLuint client_id = GetIdAllocator(IdNamespaces::kGpuFences)
                         ->AllocateIDAtOrAbove(last_gpu_fence_id_ + 1);
  CHECK(client_id > last_gpu_fence_id_) << "ID wrap prevented";
  last_gpu_fence_id_ = client_id;

  gpu_control_->CreateGpuFence(client_id, source);

  GPU_CLIENT_LOG("returned " << client_id);
  CheckGLError();
  return client_id;
}
