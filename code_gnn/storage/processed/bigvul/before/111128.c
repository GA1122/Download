bool WebPage::dispatchBeforeUnloadEvent()
{
    return d->m_page->mainFrame()->loader()->shouldClose();
}
