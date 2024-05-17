bool HTMLInputElement::recalcWillValidate() const
{
    return m_inputType->supportsValidation() && HTMLTextFormControlElement::recalcWillValidate();
}
