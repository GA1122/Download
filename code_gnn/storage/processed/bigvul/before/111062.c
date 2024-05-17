void WebPage::assignFocus(Platform::FocusDirection direction)
{
    if (d->m_page->defersLoading())
       return;
    d->assignFocus(direction);
}
