bool HTMLFormControlElement::willValidate() const
{
    if (!m_willValidateInitialized || m_dataListAncestorState == Unknown) {
        const_cast<HTMLFormControlElement*>(this)->setNeedsWillValidateCheck();
    } else {
        ASSERT(m_willValidate == recalcWillValidate());
    }
    return m_willValidate;
}
