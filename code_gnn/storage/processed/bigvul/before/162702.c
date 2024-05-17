bool BaseRenderingContext2D::RectContainsTransformedRect(
    const FloatRect& rect,
    const SkIRect& transformed_rect) const {
  FloatQuad quad(rect);
  FloatQuad transformed_quad(
      FloatRect(transformed_rect.x(), transformed_rect.y(),
                transformed_rect.width(), transformed_rect.height()));
  return GetState().Transform().MapQuad(quad).ContainsQuad(transformed_quad);
}
