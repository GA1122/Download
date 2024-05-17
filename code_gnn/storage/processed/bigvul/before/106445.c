void WebPageProxy::changeSpellingToWord(const String& word)
{
    if (word.isEmpty())
        return;

    process()->send(Messages::WebPage::ChangeSpellingToWord(word), m_pageID);
}
