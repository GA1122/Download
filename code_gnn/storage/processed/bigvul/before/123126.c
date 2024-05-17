void RenderWidgetHostViewAndroid::SetBounds(const gfx::Rect& rect) {
  if (rect.origin().x() || rect.origin().y()) {
    VLOG(0) << "SetBounds not implemented for (x,y)!=(0,0)";
  }
  SetSize(rect.size());
}
