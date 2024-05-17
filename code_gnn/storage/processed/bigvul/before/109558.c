HTMLDialogElement* Document::activeModalDialog() const
{
    if (m_topLayerElements.isEmpty())
        return 0;
    return toHTMLDialogElement(m_topLayerElements.last().get());
}
