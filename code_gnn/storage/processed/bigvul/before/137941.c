Node* AXLayoutObject::getNode() const {
  return getLayoutObject() ? getLayoutObject()->node() : nullptr;
}
