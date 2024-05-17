void TabStrip::OwnDragController(TabDragController* controller) {
  if (controller != drag_controller_.get())
    drag_controller_.reset(controller);
}
