bool AXNodeObject::canvasHasFallbackContent() const {
  Node* node = this->getNode();
  if (!isHTMLCanvasElement(node))
    return false;

  return ElementTraversal::firstChild(*node);
}
