FramebufferManager::~FramebufferManager() {
  DCHECK(framebuffers_.empty());
  CHECK_EQ(framebuffer_count_, 0u);
}
