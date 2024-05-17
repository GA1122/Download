BlackBerry::Platform::String WebPage::cutSelectedText()
{
    BlackBerry::Platform::String selectedText = d->m_selectionHandler->selectedText();
    if (!d->m_page->defersLoading() && !selectedText.empty())
        d->m_inputHandler->deleteSelection();
    return selectedText;
}
