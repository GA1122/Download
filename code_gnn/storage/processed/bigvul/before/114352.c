    ~WebGraphicsContext3DCommandBufferImpl() {
  if (gl_) {
    gl_->SetErrorMessageCallback(NULL);
  }

  if (host_) {
    if (host_->WillGpuSwitchOccur(false, gpu_preference_)) {
      host_->ForciblyCloseChannel();
      ClearSharedContexts();
    }
  }

  {
    base::AutoLock lock(g_all_shared_contexts_lock.Get());
    g_all_shared_contexts.Pointer()->erase(this);
  }
  Destroy();
}
