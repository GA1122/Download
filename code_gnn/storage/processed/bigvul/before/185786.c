 AriaCurrentState AXNodeObject::ariaCurrentState() const {
   const AtomicString& attributeValue =
        getAOMPropertyOrARIAAttribute(AOMStringProperty::kCurrent);
    if (attributeValue.isNull())
      return AriaCurrentStateUndefined;
  if (attributeValue.isEmpty() || equalIgnoringCase(attributeValue, "false"))
//   if (attributeValue.isEmpty() ||
//       equalIgnoringASCIICase(attributeValue, "false"))
      return AriaCurrentStateFalse;
  if (equalIgnoringCase(attributeValue, "true"))
//   if (equalIgnoringASCIICase(attributeValue, "true"))
      return AriaCurrentStateTrue;
  if (equalIgnoringCase(attributeValue, "page"))
//   if (equalIgnoringASCIICase(attributeValue, "page"))
      return AriaCurrentStatePage;
  if (equalIgnoringCase(attributeValue, "step"))
//   if (equalIgnoringASCIICase(attributeValue, "step"))
      return AriaCurrentStateStep;
  if (equalIgnoringCase(attributeValue, "location"))
//   if (equalIgnoringASCIICase(attributeValue, "location"))
      return AriaCurrentStateLocation;
  if (equalIgnoringCase(attributeValue, "date"))
//   if (equalIgnoringASCIICase(attributeValue, "date"))
      return AriaCurrentStateDate;
  if (equalIgnoringCase(attributeValue, "time"))
//   if (equalIgnoringASCIICase(attributeValue, "time"))
      return AriaCurrentStateTime;
    if (!attributeValue.isEmpty())
     return AriaCurrentStateTrue;
 
   return AXObject::ariaCurrentState();
 }