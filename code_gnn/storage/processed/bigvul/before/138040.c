void AXNodeObject::detach() {
  AXObject::detach();
  m_node = nullptr;
}
