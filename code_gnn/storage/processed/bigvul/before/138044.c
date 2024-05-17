Document* AXNodeObject::getDocument() const {
  if (!getNode())
    return 0;
  return &getNode()->document();
}
