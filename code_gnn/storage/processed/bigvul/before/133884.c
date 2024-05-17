void FormAssociatedElement::didMoveToNewDocument(Document& oldDocument)
{
    HTMLElement* element = toHTMLElement(this);
    if (element->fastHasAttribute(formAttr))
        m_formAttributeTargetObserver = nullptr;
}
