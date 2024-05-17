BlackBerry::Platform::String WebPage::textHasAttribute(const BlackBerry::Platform::String& query) const
{
    if (Document* doc = d->m_page->focusController()->focusedOrMainFrame()->document())
        return doc->queryCommandValue(query);

    return "";
}
