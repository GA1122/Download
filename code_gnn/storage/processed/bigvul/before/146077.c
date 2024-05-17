WebGLQuery* WebGL2RenderingContextBase::createQuery() {
  if (isContextLost())
    return nullptr;
  return WebGLQuery::Create(this);
}
