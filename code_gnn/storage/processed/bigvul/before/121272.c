bool HTMLInputElement::shouldAppearIndeterminate() const
{
    return m_inputType->supportsIndeterminateAppearance() && indeterminate();
}
