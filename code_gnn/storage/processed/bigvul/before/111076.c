bool WebPage::canGoBackOrForward(int delta) const
{
    return d->m_page->canGoBackOrForward(delta);
}
