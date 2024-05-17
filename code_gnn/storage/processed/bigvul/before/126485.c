void TabStripGtk::ContinueDrag(GdkDragContext* context) {
  if (drag_controller_.get())
    drag_controller_->Drag();
}
