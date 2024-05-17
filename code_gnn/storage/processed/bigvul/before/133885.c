HTMLFormElement* FormAssociatedElement::findAssociatedForm(const HTMLElement* element, HTMLFormElement* currentAssociatedForm)
{
    const AtomicString& formId(element->fastGetAttribute(formAttr));
    if (!formId.isNull() && element->inDocument()) {
        HTMLFormElement* newForm = 0;
        Element* newFormCandidate = element->treeScope().getElementById(formId);
        if (newFormCandidate && newFormCandidate->hasTagName(formTag))
            newForm = toHTMLFormElement(newFormCandidate);
        return newForm;
    }

    if (!currentAssociatedForm)
        return element->findFormAncestor();

    return currentAssociatedForm;
}
