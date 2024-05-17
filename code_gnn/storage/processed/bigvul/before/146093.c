WebGLSync* WebGL2RenderingContextBase::fenceSync(GLenum condition,
                                                 GLbitfield flags) {
  if (isContextLost())
    return nullptr;

  return WebGLFenceSync::Create(this, condition, flags);
}
