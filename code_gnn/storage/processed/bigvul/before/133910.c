String FormAssociatedElement::validationMessage() const
{
    return customError() ? m_customValidationMessage : String();
}
