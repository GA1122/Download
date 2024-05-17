bool Editor::TryDHTMLCopy() {
  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  if (IsInPasswordField(
          GetFrame().Selection().ComputeVisibleSelectionInDOMTree().Start()))
    return false;

  return !DispatchCPPEvent(EventTypeNames::copy, kDataTransferWritable);
}
