  void addObjectAttribute(AXObjectAttribute attribute, AXObject& object) {
    switch (attribute) {
      case AXObjectAttribute::AriaActiveDescendant:
        m_properties.addItem(
            createProperty(AXRelationshipAttributesEnum::Activedescendant,
                           createRelatedNodeListValue(object)));
        break;
      case AXObjectAttribute::AriaErrorMessage:
        m_properties.addItem(
            createProperty(AXRelationshipAttributesEnum::Errormessage,
                           createRelatedNodeListValue(object)));
        break;
    }
  }
