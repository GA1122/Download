void WebGraphicsContext3DCommandBufferImpl::drawElements(WGC3Denum mode,
                                                         WGC3Dsizei count,
                                                         WGC3Denum type,
                                                         WGC3Dintptr offset) {
  gl_->DrawElements(
      mode, count, type,
      reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
}
