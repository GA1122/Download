bool Editor::tryDHTMLCut() {
  if (frame().selection().isInPasswordField())
    return false;

  return !dispatchCPPEvent(EventTypeNames::cut, DataTransferWritable);
}
