void RenderWidgetHostImpl::SetIntersectsViewport(bool intersects) {
  if (intersects_viewport_ == intersects)
    return;

  intersects_viewport_ = intersects;
  UpdatePriority();
}
