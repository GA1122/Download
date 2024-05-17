void FormAssociatedElement::formAttributeChanged()
{
    HTMLElement* element = toHTMLElement(this);
    if (!element->fastHasAttribute(formAttr)) {
        HTMLFormElement* originalForm = m_form;
        setForm(element->findFormAncestor());
        HTMLElement* element = toHTMLElement(this);
        if (m_form && m_form != originalForm && m_form->inDocument())
            element->document().didAssociateFormControl(element);
        m_formAttributeTargetObserver = nullptr;
    } else {
        resetFormOwner();
        if (element->inDocument())
            resetFormAttributeTargetObserver();
    }
}
