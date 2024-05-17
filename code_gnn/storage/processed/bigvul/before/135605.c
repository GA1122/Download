bool Editor::TryDHTMLCut() {
  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  if (IsInPasswordField(
          GetFrame().Selection().ComputeVisibleSelectionInDOMTree().Start()))
    return false;

  return !DispatchCPPEvent(EventTypeNames::cut, kDataTransferWritable);
}
