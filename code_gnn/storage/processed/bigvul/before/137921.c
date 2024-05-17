void AXLayoutObject::clearChildren() {
  AXObject::clearChildren();
  m_childrenDirty = false;
}
