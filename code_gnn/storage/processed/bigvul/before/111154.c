bool WebPage::findNextString(const char* text, bool forward, bool caseSensitive, bool wrap, bool highlightAllMatches)
{
    WebCore::FindOptions findOptions = WebCore::StartInSelection;
    if (!forward)
        findOptions |= WebCore::Backwards;
    if (!caseSensitive)
        findOptions |= WebCore::CaseInsensitive;

    return d->m_inPageSearchManager->findNextString(String::fromUTF8(text), findOptions, wrap, highlightAllMatches);
}
