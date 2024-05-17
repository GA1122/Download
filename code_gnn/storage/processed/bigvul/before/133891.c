void FormAssociatedElement::insertedInto(ContainerNode* insertionPoint)
{
    if (m_form && insertionPoint->highestAncestor() != m_form->highestAncestor())
        setForm(0);

    resetFormOwner();
    if (!insertionPoint->inDocument())
        return;

    HTMLElement* element = toHTMLElement(this);
    if (element->fastHasAttribute(formAttr))
        resetFormAttributeTargetObserver();
}
