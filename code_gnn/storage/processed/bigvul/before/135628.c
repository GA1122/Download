void FrameSelection::DidChangeFocus() {
  DisableCompositingQueryAsserts disabler;
  UpdateAppearance();
}
