AXObject* AXNodeObject::computeParentIfExists() const {
  if (Node* parentNode = getParentNodeForComputeParent(getNode()))
    return axObjectCache().get(parentNode);

  return nullptr;
}
