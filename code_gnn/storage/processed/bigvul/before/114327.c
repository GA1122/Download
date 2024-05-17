void* WebGraphicsContext3DCommandBufferImpl::mapBufferSubDataCHROMIUM(
    WGC3Denum target,
    WGC3Dintptr offset,
    WGC3Dsizeiptr size,
    WGC3Denum access) {
  return gl_->MapBufferSubDataCHROMIUM(target, offset, size, access);
}
