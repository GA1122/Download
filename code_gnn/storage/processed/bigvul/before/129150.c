void FramebufferManager::Destroy(bool have_context) {
  have_context_ = have_context;
  framebuffers_.clear();
}
