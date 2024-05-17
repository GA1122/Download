std::unique_ptr<TracedValue> InspectorStyleInvalidatorInvalidateEvent::Data(
    Element& element,
    const char* reason) {
  return FillCommonPart(element, reason);
}
