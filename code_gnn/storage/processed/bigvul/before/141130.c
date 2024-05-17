bool Document::IsInMainFrame() const {
  return GetFrame() && GetFrame()->IsMainFrame();
}
