void Document::removeFormAssociation(Element* element)
{
    auto it = m_associatedFormControls.find(element);
    if (it == m_associatedFormControls.end())
        return;
    m_associatedFormControls.remove(it);
    if (m_associatedFormControls.isEmpty())
        m_didAssociateFormControlsTimer.stop();
}
