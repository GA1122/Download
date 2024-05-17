void HTMLSelectElement::updateListItemSelectedStates()
{
    if (!m_shouldRecalcListItems)
        return;
    recalcListItems();
    setNeedsValidityCheck();
}
