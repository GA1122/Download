void WebGLRenderingContextBase::depthRange(GLfloat z_near, GLfloat z_far) {
  if (isContextLost())
    return;
  if (z_near > z_far) {
    SynthesizeGLError(GL_INVALID_OPERATION, "depthRange", "zNear > zFar");
    return;
  }
  ContextGL()->DepthRangef(z_near, z_far);
}
