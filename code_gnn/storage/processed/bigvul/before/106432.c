void WebPageProxy::backForwardAddItem(uint64_t itemID)
{
    m_backForwardList->addItem(process()->webBackForwardItem(itemID));
}
