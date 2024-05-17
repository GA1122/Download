bool Editor::CanDHTMLCopy() {
  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  return !IsInPasswordField(GetFrame()
                                .Selection()
                                .ComputeVisibleSelectionInDOMTree()
                                .Start()) &&
         !DispatchCPPEvent(EventTypeNames::beforecopy, kDataTransferNumb);
}
