void OmniboxViewViews::RemovedFromWidget() {
  views::Textfield::RemovedFromWidget();
  scoped_compositor_observer_.RemoveAll();
}
