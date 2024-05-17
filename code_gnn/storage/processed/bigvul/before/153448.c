bool TabStrip::OnMouseWheel(const ui::MouseWheelEvent& event) {
  if ((!event.x_offset() && !event.IsShiftDown()) || tab_count() < 2 ||
      !FindTabHitByPoint(event.location()))
    return false;

  accumulated_horizontal_scroll_ +=
      (event.x_offset() ? event.x_offset() : event.y_offset());

  int horizontal_offset =
      accumulated_horizontal_scroll_ / ui::MouseWheelEvent::kWheelDelta;
  if (!horizontal_offset)
    return true;

  accumulated_horizontal_scroll_ %= ui::MouseWheelEvent::kWheelDelta;

  int new_active_index =
      (controller_->GetActiveIndex() + horizontal_offset) % tab_count();
  if (new_active_index < 0)
    new_active_index += tab_count();

  DCHECK(IsValidModelIndex(new_active_index));
  controller_->SelectTab(new_active_index, event);
  return true;
}
