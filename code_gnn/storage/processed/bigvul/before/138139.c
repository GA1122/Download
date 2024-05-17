String AXObject::computedName() const {
  AXNameFrom nameFrom;
  AXObject::AXObjectVector nameObjects;
  return name(nameFrom, &nameObjects);
}
