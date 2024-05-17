void ShellWindowViews::SetDraggableRegion(SkRegion* region) {
  caption_region_.Set(region);
  OnViewWasResized();
}
