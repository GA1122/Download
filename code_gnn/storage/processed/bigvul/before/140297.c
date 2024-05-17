bool Editor::canDHTMLCopy() {
  return !frame().selection().isInPasswordField() &&
         !dispatchCPPEvent(EventTypeNames::beforecopy, DataTransferNumb);
}
