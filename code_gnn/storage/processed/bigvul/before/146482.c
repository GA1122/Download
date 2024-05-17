void WebGLRenderingContextBase::depthMask(GLboolean flag) {
  if (isContextLost())
    return;
  depth_mask_ = flag;
  ContextGL()->DepthMask(flag);
}
