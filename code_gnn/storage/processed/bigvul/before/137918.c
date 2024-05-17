void AXLayoutObject::ariaOwnsElements(AXObjectVector& owns) const {
  accessibilityChildrenFromAttribute(aria_ownsAttr, owns);
}
