DocumentState* Document::formElementsState() const
{
    if (!m_formController)
        return 0;
    return m_formController->formElementsState();
}
