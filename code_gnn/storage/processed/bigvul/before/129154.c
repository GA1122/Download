bool FramebufferManager::GetClientId(
    GLuint service_id, GLuint* client_id) const {
  for (FramebufferMap::const_iterator it = framebuffers_.begin();
       it != framebuffers_.end(); ++it) {
    if (it->second->service_id() == service_id) {
      *client_id = it->first;
      return true;
    }
  }
  return false;
}
