void TabStrip::DestroyDragController() {
  newtab_button_->SetVisible(true);
  drag_controller_.reset();
}
