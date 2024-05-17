AXObject* AXObject::leafNodeAncestor() const {
  if (AXObject* parent = parentObject()) {
    if (!parent->canHaveChildren())
      return parent;

    return parent->leafNodeAncestor();
  }

  return 0;
}
