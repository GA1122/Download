void WebPage::setTimeoutForJavaScriptExecution(unsigned ms)
{
    Settings::setTimeoutForJavaScriptExecution(d->m_page->groupName(), ms);

    Document* doc = d->m_page->mainFrame()->document();
    if (!doc)
        return;

    doc->globalData()->timeoutChecker.setTimeoutInterval(ms);
}
