void WebPageProxy::advanceToNextMisspelling(bool startBeforeSelection)
{
    process()->send(Messages::WebPage::AdvanceToNextMisspelling(startBeforeSelection), m_pageID);
}
