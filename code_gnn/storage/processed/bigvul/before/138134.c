void AXObject::clearChildren() {
  for (const auto& child : m_children)
    child->detachFromParent();

  m_children.clear();
  m_haveChildren = false;
}
