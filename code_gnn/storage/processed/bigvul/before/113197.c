void LauncherView::MousePressedOnButton(views::View* view,
                                        const views::MouseEvent& event) {
  tooltip_->Close();
  int index = view_model_->GetIndexOfView(view);
  if (index == -1 ||
      view_model_->view_size() <= 1 ||
      !delegate_->IsDraggable(model_->items()[index]))
    return;   

  drag_view_ = view;
  drag_offset_ = primary_axis_coordinate(event.x(), event.y());
}
