bool WebGraphicsContext3DCommandBufferImpl::readBackFramebuffer(
    unsigned char* pixels,
    size_t buffer_size) {
  return readBackFramebuffer(pixels, buffer_size, 0, width(), height());
}
