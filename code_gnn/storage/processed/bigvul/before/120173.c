void Layer::SetNeedsDisplayRect(const gfx::RectF& dirty_rect) {
  if (dirty_rect.IsEmpty())
    return;

  SetNeedsPushProperties();
  update_rect_.Union(dirty_rect);

  if (DrawsContent())
    SetNeedsUpdate();
}
