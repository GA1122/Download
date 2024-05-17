void AXObjectCacheImpl::labelChanged(Element* element) {
  textChanged(toHTMLLabelElement(element)->control());
}
