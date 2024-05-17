void AXLayoutObject::addRemoteSVGChildren() {
  AXSVGRoot* root = remoteSVGRootElement();
  if (!root)
    return;

  root->setParent(this);

  if (root->accessibilityIsIgnored()) {
    for (const auto& child : root->children())
      m_children.push_back(child);
  } else {
    m_children.push_back(root);
  }
}
