void HTMLFormControlElement::findCustomValidationMessageTextDirection(
    const String& message,
    TextDirection& messageDir,
    String& subMessage,
    TextDirection& subMessageDir) {
  messageDir = determineDirectionality(message);
  if (!subMessage.isEmpty())
    subMessageDir = layoutObject()->style()->direction();
}
