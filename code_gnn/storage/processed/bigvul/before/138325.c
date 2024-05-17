void fillLiveRegionProperties(AXObject& axObject,
                              protocol::Array<AXProperty>& properties) {
  if (!axObject.liveRegionRoot())
    return;

  properties.addItem(
      createProperty(AXLiveRegionAttributesEnum::Live,
                     createValue(axObject.containerLiveRegionStatus(),
                                 AXValueTypeEnum::Token)));
  properties.addItem(
      createProperty(AXLiveRegionAttributesEnum::Atomic,
                     createBooleanValue(axObject.containerLiveRegionAtomic())));
  properties.addItem(
      createProperty(AXLiveRegionAttributesEnum::Relevant,
                     createValue(axObject.containerLiveRegionRelevant(),
                                 AXValueTypeEnum::TokenList)));
  properties.addItem(
      createProperty(AXLiveRegionAttributesEnum::Busy,
                     createBooleanValue(axObject.containerLiveRegionBusy())));

  if (!axObject.isLiveRegion()) {
    properties.addItem(createProperty(
        AXLiveRegionAttributesEnum::Root,
        createRelatedNodeListValue(*(axObject.liveRegionRoot()))));
  }
}
