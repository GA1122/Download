static bool LayerNodeMayNeedCompositedScrolling(const PaintLayer* layer) {
  if (Node* node = layer->GetLayoutObject().GetNode()) {
    if (IsHTMLSelectElement(node))
      return false;
    if (TextControlElement* text_control = EnclosingTextControl(node)) {
      if (IsHTMLInputElement(text_control)) {
        return false;
      }
    }
  }
  return true;
}
