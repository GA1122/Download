bool HTMLInputElement::isOutOfRange() const
{
    return m_inputType->isOutOfRange(value());
}
