bool TabStrip::IsStackingDraggedTabs() const {
  return drag_controller_.get() && drag_controller_->started_drag() &&
      (drag_controller_->move_behavior() ==
       TabDragController::MOVE_VISIBILE_TABS);
}
