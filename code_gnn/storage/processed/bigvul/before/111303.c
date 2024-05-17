BlackBerry::Platform::String WebPage::renderTreeAsText()
{
    return externalRepresentation(d->m_mainFrame);
}
