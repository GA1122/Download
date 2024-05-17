bool BaseRenderingContext2D::ComputeDirtyRect(
    const FloatRect& local_rect,
    const SkIRect& transformed_clip_bounds,
    SkIRect* dirty_rect) {
  FloatRect canvas_rect = GetState().Transform().MapRect(local_rect);

  if (AlphaChannel(GetState().ShadowColor())) {
    FloatRect shadow_rect(canvas_rect);
    shadow_rect.Move(GetState().ShadowOffset());
    shadow_rect.Inflate(GetState().ShadowBlur());
    canvas_rect.Unite(shadow_rect);
  }

  SkIRect canvas_i_rect;
  static_cast<SkRect>(canvas_rect).roundOut(&canvas_i_rect);
  if (!canvas_i_rect.intersect(transformed_clip_bounds))
    return false;

  if (dirty_rect)
    *dirty_rect = canvas_i_rect;

  return true;
}
