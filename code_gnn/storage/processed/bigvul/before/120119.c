gfx::Rect Layer::LayerRectToContentRect(const gfx::RectF& layer_rect) const {
  gfx::RectF content_rect =
      gfx::ScaleRect(layer_rect, contents_scale_x(), contents_scale_y());
  content_rect.Intersect(gfx::Rect(content_bounds()));
  return gfx::ToEnclosingRect(content_rect);
}
