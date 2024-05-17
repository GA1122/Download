DEFINE_TRACE(AXNodeObject) {
  visitor->trace(m_node);
  AXObject::trace(visitor);
}
