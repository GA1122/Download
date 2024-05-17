bool Editor::canDHTMLCut() {
  return !frame().selection().isInPasswordField() &&
         !dispatchCPPEvent(EventTypeNames::beforecut, DataTransferNumb);
}
