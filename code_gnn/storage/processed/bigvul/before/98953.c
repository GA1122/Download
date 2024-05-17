void HTMLLinkElement::setDisabledState(bool _disabled)
{
    DisabledState oldDisabledState = m_disabledState;
    m_disabledState = _disabled ? Disabled : EnabledViaScript;
    if (oldDisabledState != m_disabledState) {
        if (isLoading()) {
            if (m_disabledState == Disabled && (!m_relAttribute.m_isAlternate || oldDisabledState == EnabledViaScript))
                document()->removePendingSheet();

            if (m_relAttribute.m_isAlternate && m_disabledState == EnabledViaScript)
                document()->addPendingSheet();

            if (!m_relAttribute.m_isAlternate && m_disabledState == EnabledViaScript && oldDisabledState == Disabled)
                document()->addPendingSheet();

            return;
        }

        if (!m_sheet && m_disabledState == EnabledViaScript)
            process();
        else
            document()->updateStyleSelector();  
    }
}
