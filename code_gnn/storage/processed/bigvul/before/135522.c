bool Editor::CanDHTMLCut() {
  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  return !IsInPasswordField(GetFrame()
                                .Selection()
                                .ComputeVisibleSelectionInDOMTree()
                                .Start()) &&
         !DispatchCPPEvent(EventTypeNames::beforecut, kDataTransferNumb);
}
