Vector<String> HTMLInputElement::acceptFileExtensions()
{
    return parseAcceptAttribute(fastGetAttribute(acceptAttr), isValidFileExtension);
}
