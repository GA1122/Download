void AXNodeObject::decrement() {
  UserGestureIndicator gestureIndicator(DocumentUserGestureToken::create(
      getDocument(), UserGestureToken::NewGesture));
  alterSliderValue(false);
}
