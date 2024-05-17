bool Editor::tryDHTMLCopy() {
  if (frame().selection().isInPasswordField())
    return false;

  return !dispatchCPPEvent(EventTypeNames::copy, DataTransferWritable);
}
