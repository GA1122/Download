Framebuffer* FramebufferManager::GetFramebuffer(
    GLuint client_id) {
  FramebufferMap::iterator it = framebuffers_.find(client_id);
  return it != framebuffers_.end() ? it->second.get() : NULL;
}
