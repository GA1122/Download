void fillGlobalStates(AXObject& axObject,
                      protocol::Array<AXProperty>& properties) {
  if (!axObject.isEnabled())
    properties.addItem(
        createProperty(AXGlobalStatesEnum::Disabled, createBooleanValue(true)));

  if (const AXObject* hiddenRoot = axObject.ariaHiddenRoot()) {
    properties.addItem(
        createProperty(AXGlobalStatesEnum::Hidden, createBooleanValue(true)));
    properties.addItem(createProperty(AXGlobalStatesEnum::HiddenRoot,
                                      createRelatedNodeListValue(*hiddenRoot)));
  }

  InvalidState invalidState = axObject.getInvalidState();
  switch (invalidState) {
    case InvalidStateUndefined:
      break;
    case InvalidStateFalse:
      properties.addItem(
          createProperty(AXGlobalStatesEnum::Invalid,
                         createValue("false", AXValueTypeEnum::Token)));
      break;
    case InvalidStateTrue:
      properties.addItem(
          createProperty(AXGlobalStatesEnum::Invalid,
                         createValue("true", AXValueTypeEnum::Token)));
      break;
    case InvalidStateSpelling:
      properties.addItem(
          createProperty(AXGlobalStatesEnum::Invalid,
                         createValue("spelling", AXValueTypeEnum::Token)));
      break;
    case InvalidStateGrammar:
      properties.addItem(
          createProperty(AXGlobalStatesEnum::Invalid,
                         createValue("grammar", AXValueTypeEnum::Token)));
      break;
    default:
      properties.addItem(createProperty(
          AXGlobalStatesEnum::Invalid,
          createValue(axObject.ariaInvalidValue(), AXValueTypeEnum::String)));
      break;
  }
}
