bool WebPage::goBackOrForward(int delta)
{
    if (d->m_page->canGoBackOrForward(delta)) {
        d->m_backingStore->d->suspendScreenUpdates();
        d->m_page->goBackOrForward(delta);
        d->m_backingStore->d->resumeScreenUpdates(BackingStore::None);
        return true;
    }
    return false;
}
