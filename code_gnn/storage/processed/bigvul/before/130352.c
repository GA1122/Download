void HTMLFormControlElement::findCustomValidationMessageTextDirection(const String& message, TextDirection &messageDir, String& subMessage, TextDirection &subMessageDir)
{
    subMessage = fastGetAttribute(titleAttr);
    messageDir = determineDirectionality(message);
    if (!subMessage.isEmpty())
        subMessageDir = layoutObject()->style()->direction();
}
