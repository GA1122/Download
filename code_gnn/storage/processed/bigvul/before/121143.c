void HTMLInputElement::detach()
{
    HTMLTextFormControlElement::detach();
    setFormControlValueMatchesRenderer(false);
    m_inputType->detach();
}
