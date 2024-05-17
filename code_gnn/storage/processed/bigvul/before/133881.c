bool FormAssociatedElement::customError() const
{
    const HTMLElement* element = toHTMLElement(this);
    return element->willValidate() && !m_customValidationMessage.isEmpty();
}
