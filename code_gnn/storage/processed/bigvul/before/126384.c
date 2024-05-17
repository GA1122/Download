bool BrowserWindowGtk::IsTabStripEditable() const {
  return !tabstrip()->IsDragSessionActive() &&
      !tabstrip()->IsActiveDropTarget();
}
