void HTMLCanvasElement::DidDraw() {
  DidDraw(FloatRect(0, 0, Size().Width(), Size().Height()));
}
