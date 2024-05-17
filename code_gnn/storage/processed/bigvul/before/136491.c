FloatRect EffectPaintPropertyNode::MapRect(const FloatRect& input_rect) const {
  FloatRect rect = input_rect;
  rect.MoveBy(-state_.paint_offset);
  FloatRect result = state_.filter.MapRect(rect);
  result.MoveBy(state_.paint_offset);
  return result;
}
