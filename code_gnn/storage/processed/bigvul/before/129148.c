void FramebufferManager::CreateFramebuffer(
    GLuint client_id, GLuint service_id) {
  std::pair<FramebufferMap::iterator, bool> result =
      framebuffers_.insert(
          std::make_pair(
              client_id,
              scoped_refptr<Framebuffer>(
                  new Framebuffer(this, service_id))));
  DCHECK(result.second);
}
