bool WebGLRenderingContextBase::IsOriginTopLeft() const {
  if (isContextLost())
    return false;
  return is_origin_top_left_;
}
