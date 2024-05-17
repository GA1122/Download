bool DateTimeFieldElement::isFocusable() const
{
    if (isReadOnly())
        return false;
    if (m_fieldOwner && m_fieldOwner->isFieldOwnerDisabledOrReadOnly())
        return false;
    return HTMLElement::isFocusable();
}
