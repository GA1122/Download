void AXNodeObject::insertChild(AXObject* child, unsigned index) {
  if (!child)
    return;

  child->clearChildren();

  if (child->accessibilityIsIgnored()) {
    const auto& children = child->children();
    size_t length = children.size();
    for (size_t i = 0; i < length; ++i)
      m_children.insert(index + i, children[i]);
  } else {
    ASSERT(child->parentObject() == this);
    m_children.insert(index, child);
  }
}
