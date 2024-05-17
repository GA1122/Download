bool InputHandler::PointIsWithinContents(gfx::PointF point) const {
  gfx::Rect bounds = host_->GetView()->GetViewBounds();
  bounds -= bounds.OffsetFromOrigin();   
  return bounds.Contains(point.x(), point.y());
}
