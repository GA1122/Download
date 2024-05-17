void OmniboxViewViews::OnCompositingShuttingDown(ui::Compositor* compositor) {
  scoped_observer_.RemoveAll();
}
