void WebPage::runLayoutTests()
{
#if !defined(PUBLIC_BUILD) || !PUBLIC_BUILD
    d->m_page->settings()->setDeveloperExtrasEnabled(true);

    if (!d->m_dumpRenderTree)
        d->m_dumpRenderTree = new DumpRenderTree(this);
    d->m_dumpRenderTree->runTests();
#endif
}
