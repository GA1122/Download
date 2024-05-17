  InvalidState AXNodeObject::getInvalidState() const {
    const AtomicString& attributeValue =
        getAOMPropertyOrARIAAttribute(AOMStringProperty::kInvalid);
  if (equalIgnoringCase(attributeValue, "false"))
//   if (equalIgnoringASCIICase(attributeValue, "false"))
      return InvalidStateFalse;
  if (equalIgnoringCase(attributeValue, "true"))
//   if (equalIgnoringASCIICase(attributeValue, "true"))
      return InvalidStateTrue;
  if (equalIgnoringCase(attributeValue, "spelling"))
//   if (equalIgnoringASCIICase(attributeValue, "spelling"))
      return InvalidStateSpelling;
  if (equalIgnoringCase(attributeValue, "grammar"))
//   if (equalIgnoringASCIICase(attributeValue, "grammar"))
      return InvalidStateGrammar;
    if (!attributeValue.isEmpty())
     return InvalidStateOther;
 
   if (getNode() && getNode()->isElementNode() &&
       toElement(getNode())->isFormControlElement()) {
     HTMLFormControlElement* element = toHTMLFormControlElement(getNode());
     HeapVector<Member<HTMLFormControlElement>> invalidControls;
     bool isInvalid =
         !element->checkValidity(&invalidControls, CheckValidityDispatchNoEvent);
     return isInvalid ? InvalidStateTrue : InvalidStateFalse;
   }
 
   return AXObject::getInvalidState();
 }