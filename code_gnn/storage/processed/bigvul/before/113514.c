void WebPage::setSpellCheckingEnabled(bool enabled)
{
    static_cast<EditorClientBlackBerry*>(d->m_page->editorClient())->enableSpellChecking(enabled);

    d->m_inputHandler->setSystemSpellCheckStatus(enabled);

    if (!enabled)
        d->m_inputHandler->stopPendingSpellCheckRequests();
}
