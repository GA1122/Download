bool AXObject::press() {
  Document* document = getDocument();
  if (!document)
    return false;

  UserGestureIndicator gestureIndicator(
      DocumentUserGestureToken::create(document, UserGestureToken::NewGesture));
  Element* actionElem = actionElement();
  if (actionElem) {
    actionElem->accessKeyAction(true);
    return true;
  }

  if (canSetFocusAttribute()) {
    setFocused(true);
    return true;
  }

  return false;
}
