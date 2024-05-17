float LayerTreeHostImpl::DeviceSpaceDistanceToLayer(
    const gfx::PointF& device_viewport_point,
    LayerImpl* layer_impl) {
  if (!layer_impl)
    return std::numeric_limits<float>::max();

  gfx::Rect layer_impl_bounds(layer_impl->bounds());

  gfx::RectF device_viewport_layer_impl_bounds = MathUtil::MapClippedRect(
      layer_impl->ScreenSpaceTransform(), gfx::RectF(layer_impl_bounds));

  return device_viewport_layer_impl_bounds.ManhattanDistanceToPoint(
      device_viewport_point);
}
