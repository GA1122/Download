void FramebufferManager::RemoveFramebuffer(GLuint client_id) {
  FramebufferMap::iterator it = framebuffers_.find(client_id);
  if (it != framebuffers_.end()) {
    it->second->MarkAsDeleted();
    framebuffers_.erase(it);
  }
}
