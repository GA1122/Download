WebGLVertexArrayObject* WebGL2RenderingContextBase::createVertexArray() {
  if (isContextLost())
    return nullptr;

  return WebGLVertexArrayObject::Create(
      this, WebGLVertexArrayObjectBase::kVaoTypeUser);
}
