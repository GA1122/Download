bool HTMLInputElement::isInRange() const
{
    return m_inputType->isInRange(value());
}
