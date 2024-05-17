VisibleSelection FrameSelection::ComputeVisibleSelectionInDOMTreeDeprecated()
    const {
  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();
  return ComputeVisibleSelectionInDOMTree();
}
