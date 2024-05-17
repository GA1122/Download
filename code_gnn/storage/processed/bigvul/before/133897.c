void FormAssociatedElement::removedFrom(ContainerNode* insertionPoint)
{
    HTMLElement* element = toHTMLElement(this);
    if (insertionPoint->inDocument() && element->fastHasAttribute(formAttr))
        m_formAttributeTargetObserver = nullptr;
    if (m_form && element->highestAncestor() != m_form->highestAncestor())
        setForm(0);
}
