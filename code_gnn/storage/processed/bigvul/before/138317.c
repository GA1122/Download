  void addStringAttribute(AXStringAttribute attribute, const String& value) {
    switch (attribute) {
      case AXStringAttribute::AriaKeyShortcuts:
        m_properties.addItem(
            createProperty(AXGlobalStatesEnum::Keyshortcuts,
                           createValue(value, AXValueTypeEnum::String)));
        break;
      case AXStringAttribute::AriaRoleDescription:
        m_properties.addItem(
            createProperty(AXGlobalStatesEnum::Roledescription,
                           createValue(value, AXValueTypeEnum::String)));
        break;
    }
  }
