Document* AXObject::getDocument() const {
  FrameView* frameView = documentFrameView();
  if (!frameView)
    return 0;

  return frameView->frame().document();
}
