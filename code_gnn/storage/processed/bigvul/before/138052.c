void AXNodeObject::increment() {
  UserGestureIndicator gestureIndicator(DocumentUserGestureToken::create(
      getDocument(), UserGestureToken::NewGesture));
  alterSliderValue(true);
}
