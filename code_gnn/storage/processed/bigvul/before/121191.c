bool HTMLInputElement::isSuccessfulSubmitButton() const
{
    return !isDisabledFormControl() && m_inputType->canBeSuccessfulSubmitButton();
}
