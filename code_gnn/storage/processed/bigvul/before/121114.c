Vector<String> HTMLInputElement::acceptMIMETypes()
{
    return parseAcceptAttribute(fastGetAttribute(acceptAttr), isValidMIMEType);
}
