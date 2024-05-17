void WebPageProxy::initializeWebPage()
{
    ASSERT(isValid());

    BackForwardListItemVector items = m_backForwardList->entries();
    for (size_t i = 0; i < items.size(); ++i)
        process()->registerNewWebBackForwardListItem(items[i].get());

    m_drawingArea = m_pageClient->createDrawingAreaProxy();
    ASSERT(m_drawingArea);

    process()->send(Messages::WebProcess::CreateWebPage(m_pageID, creationParameters()), 0);
}
