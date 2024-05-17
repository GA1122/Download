void WebPageProxy::loadPlainTextString(const String& string)
{
    if (!isValid())
        reattachToWebProcess();

    process()->send(Messages::WebPage::LoadPlainTextString(string), m_pageID);
}
