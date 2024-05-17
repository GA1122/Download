void WebPage::setSpellCheckingEnabled(bool enabled)
{
    static_cast<EditorClientBlackBerry*>(d->m_page->editorClient())->enableSpellChecking(enabled);
}
