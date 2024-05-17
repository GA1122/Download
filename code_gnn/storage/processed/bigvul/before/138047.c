void AXNodeObject::getSparseAXAttributes(
    AXSparseAttributeClient& sparseAttributeClient) const {
  Node* node = this->getNode();
  if (!node || !node->isElementNode())
    return;

  AXSparseAttributeSetterMap& axSparseAttributeSetterMap =
      getSparseAttributeSetterMap();
  AttributeCollection attributes = toElement(node)->attributesWithoutUpdate();
  for (const Attribute& attr : attributes) {
    SparseAttributeSetter* setter = axSparseAttributeSetterMap.at(attr.name());
    if (setter)
      setter->run(*this, sparseAttributeClient, attr.value());
  }

  if (toElement(node)->existingAccessibleNode()) {
    AtomicString keyShortcuts =
        getAOMPropertyOrARIAAttribute(AOMStringProperty::kKeyShortcuts);
    if (!keyShortcuts.isNull()) {
      axSparseAttributeSetterMap.at(aria_keyshortcutsAttr)
          ->run(*this, sparseAttributeClient, keyShortcuts);
    }
    AtomicString roleDescription =
        getAOMPropertyOrARIAAttribute(AOMStringProperty::kRoleDescription);
    if (!roleDescription.isNull()) {
      axSparseAttributeSetterMap.at(aria_roledescriptionAttr)
          ->run(*this, sparseAttributeClient, roleDescription);
    }
  }
}
