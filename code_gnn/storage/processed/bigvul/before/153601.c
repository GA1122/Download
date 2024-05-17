GLuint GLES2Implementation::CreateGpuFenceCHROMIUM() {
  GLuint client_id = GetIdAllocator(IdNamespaces::kGpuFences)
                         ->AllocateIDAtOrAbove(last_gpu_fence_id_ + 1);
  CHECK(client_id > last_gpu_fence_id_) << "ID wrap prevented";
  last_gpu_fence_id_ = client_id;
  helper_->CreateGpuFenceINTERNAL(client_id);
  GPU_CLIENT_LOG("returned " << client_id);
  CheckGLError();
  return client_id;
}
