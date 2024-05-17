 Element* elementFromCenter(Element& element) {
  ClientRect* clientRect = element.getBoundingClientRect();
  int centerX =
      static_cast<int>((clientRect->left() + clientRect->right()) / 2);
  int centerY =
      static_cast<int>((clientRect->top() + clientRect->bottom()) / 2);

  return element.document().elementFromPoint(centerX, centerY);
}
