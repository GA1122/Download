AXObject* AXObject::parentObjectUnignored() const {
  AXObject* parent;
  for (parent = parentObject(); parent && parent->accessibilityIsIgnored();
       parent = parent->parentObject()) {
  }

  return parent;
}
