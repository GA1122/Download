void WebGLRenderingContextBase::sampleCoverage(GLfloat value,
                                               GLboolean invert) {
  if (isContextLost())
    return;
  ContextGL()->SampleCoverage(value, invert);
}
