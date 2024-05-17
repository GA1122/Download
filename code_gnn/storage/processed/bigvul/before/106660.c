void WebPageProxy::unmarkAllBadGrammar()
{
    process()->send(Messages::WebPage::UnmarkAllBadGrammar(), m_pageID);
}
