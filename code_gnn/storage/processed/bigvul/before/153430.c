bool TabStrip::IsRectInWindowCaption(const gfx::Rect& rect) {
  const views::View* v = GetEventHandlerForRect(rect);
  if (v == this)
    return true;

  const int drag_handle_extension = TabStyle::GetDragHandleExtension(height());

  bool extend_drag_handle = !controller_->IsFrameCondensed() &&
                            !controller_->EverHasVisibleBackgroundTabShapes();

  const int tab_index = tabs_.GetIndexOfView(v);
  if (IsValidModelIndex(tab_index)) {
    Tab* tab = tab_at(tab_index);
    gfx::Rect tab_drag_handle = tab->GetMirroredBounds();
    tab_drag_handle.set_height(drag_handle_extension);
    return extend_drag_handle && !tab->IsActive() &&
           tab_drag_handle.Intersects(rect);
  }

  gfx::Rect new_tab_button_drag_handle = new_tab_button_->GetMirroredBounds();
  new_tab_button_drag_handle.set_height(drag_handle_extension);
  if (extend_drag_handle && new_tab_button_drag_handle.Intersects(rect))
    return true;

  gfx::RectF rect_in_new_tab_coords_f(rect);
  View::ConvertRectToTarget(this, new_tab_button_, &rect_in_new_tab_coords_f);
  gfx::Rect rect_in_new_tab_coords =
      gfx::ToEnclosingRect(rect_in_new_tab_coords_f);
  return new_tab_button_->GetLocalBounds().Intersects(rect_in_new_tab_coords) &&
         !new_tab_button_->HitTestRect(rect_in_new_tab_coords);
}
