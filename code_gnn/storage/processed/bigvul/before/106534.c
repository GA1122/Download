 void WebPageProxy::gestureDidEnd()
{
    process()->send(Messages::WebPage::GestureDidEnd(), m_pageID);
}
