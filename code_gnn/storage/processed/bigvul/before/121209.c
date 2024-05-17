bool HTMLInputElement::matchesReadOnlyPseudoClass() const
{
    return m_inputType->supportsReadOnly() && isReadOnly();
}
