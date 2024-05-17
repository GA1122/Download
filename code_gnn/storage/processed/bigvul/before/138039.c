String AXNodeObject::description(AXNameFrom nameFrom,
                                 AXDescriptionFrom& descriptionFrom,
                                 AXObjectVector* descriptionObjects) const {
  AXRelatedObjectVector relatedObjects;
  String result =
      description(nameFrom, descriptionFrom, nullptr, &relatedObjects);
  if (descriptionObjects) {
    descriptionObjects->clear();
    for (size_t i = 0; i < relatedObjects.size(); i++)
      descriptionObjects->push_back(relatedObjects[i]->object);
  }

  return collapseWhitespace(result);
}
