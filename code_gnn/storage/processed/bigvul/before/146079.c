WebGLTransformFeedback* WebGL2RenderingContextBase::createTransformFeedback() {
  if (isContextLost())
    return nullptr;
  return WebGLTransformFeedback::Create(this,
                                        WebGLTransformFeedback::TFTypeUser);
}
