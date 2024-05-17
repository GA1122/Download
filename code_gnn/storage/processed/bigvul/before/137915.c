void AXLayoutObject::ariaDescribedbyElements(
    AXObjectVector& describedby) const {
  accessibilityChildrenFromAttribute(aria_describedbyAttr, describedby);
}
