std::unique_ptr<AXNode> InspectorAccessibilityAgent::buildProtocolAXObject(
    AXObject& axObject,
    AXObject* inspectedAXObject,
    bool fetchRelatives,
    std::unique_ptr<protocol::Array<AXNode>>& nodes,
    AXObjectCacheImpl& cache) const {
  AccessibilityRole role = axObject.roleValue();
  std::unique_ptr<AXNode> nodeObject =
      AXNode::create()
          .setNodeId(String::number(axObject.axObjectID()))
          .setIgnored(false)
          .build();
  nodeObject->setRole(createRoleNameValue(role));

  std::unique_ptr<protocol::Array<AXProperty>> properties =
      protocol::Array<AXProperty>::create();
  fillLiveRegionProperties(axObject, *(properties.get()));
  fillGlobalStates(axObject, *(properties.get()));
  fillWidgetProperties(axObject, *(properties.get()));
  fillWidgetStates(axObject, *(properties.get()));
  fillRelationships(axObject, *(properties.get()));

  SparseAttributeAXPropertyAdapter adapter(axObject, *properties);
  axObject.getSparseAXAttributes(adapter);

  AXObject::NameSources nameSources;
  String computedName = axObject.name(&nameSources);
  if (!nameSources.isEmpty()) {
    std::unique_ptr<AXValue> name =
        createValue(computedName, AXValueTypeEnum::ComputedString);
    if (!nameSources.isEmpty()) {
      std::unique_ptr<protocol::Array<AXValueSource>> nameSourceProperties =
          protocol::Array<AXValueSource>::create();
      for (size_t i = 0; i < nameSources.size(); ++i) {
        NameSource& nameSource = nameSources[i];
        nameSourceProperties->addItem(createValueSource(nameSource));
        if (nameSource.text.isNull() || nameSource.superseded)
          continue;
        if (!nameSource.relatedObjects.isEmpty()) {
          properties->addItem(createRelatedNodeListProperty(
              AXRelationshipAttributesEnum::Labelledby,
              nameSource.relatedObjects));
        }
      }
      name->setSources(std::move(nameSourceProperties));
    }
    nodeObject->setProperties(std::move(properties));
    nodeObject->setName(std::move(name));
  } else {
    nodeObject->setProperties(std::move(properties));
  }

  fillCoreProperties(axObject, inspectedAXObject, fetchRelatives,
                     *(nodeObject.get()), nodes, cache);
  return nodeObject;
}
