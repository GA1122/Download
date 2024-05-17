bool TabStrip::IsRectInWindowCaption(const gfx::Rect& rect) {
  views::View* v = GetEventHandlerForRect(rect);

  if (v == this)
    return true;

  gfx::RectF rect_in_newtab_coords_f(rect);
  View::ConvertRectToTarget(this, newtab_button_, &rect_in_newtab_coords_f);
  gfx::Rect rect_in_newtab_coords = gfx::ToEnclosingRect(
      rect_in_newtab_coords_f);
  if (newtab_button_->GetLocalBounds().Intersects(rect_in_newtab_coords) &&
      !newtab_button_->HitTestRect(rect_in_newtab_coords))
    return true;

  return false;
}
