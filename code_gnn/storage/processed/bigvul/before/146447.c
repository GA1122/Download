void WebGLRenderingContextBase::blendColor(GLfloat red,
                                           GLfloat green,
                                           GLfloat blue,
                                           GLfloat alpha) {
  if (isContextLost())
    return;
  ContextGL()->BlendColor(red, green, blue, alpha);
}
