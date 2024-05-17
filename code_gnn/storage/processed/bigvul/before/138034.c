AXObject* AXNodeObject::computeParent() const {
  ASSERT(!isDetached());
  if (Node* parentNode = getParentNodeForComputeParent(getNode()))
    return axObjectCache().getOrCreate(parentNode);

  return nullptr;
}
