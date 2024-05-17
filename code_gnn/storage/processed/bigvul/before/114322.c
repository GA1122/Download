WGC3Dsizeiptr WebGraphicsContext3DCommandBufferImpl::getVertexAttribOffset(
    WGC3Duint index, WGC3Denum pname) {
  GLvoid* value = NULL;
  gl_->GetVertexAttribPointerv(index, pname, &value);
  return static_cast<WGC3Dsizeiptr>(reinterpret_cast<intptr_t>(value));
}
