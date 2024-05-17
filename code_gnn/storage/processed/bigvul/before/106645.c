void WebPageProxy::shouldGoToBackForwardListItem(uint64_t itemID, bool& shouldGoToBackForwardItem)
{
    WebBackForwardListItem* item = process()->webBackForwardItem(itemID);
    shouldGoToBackForwardItem = item && m_loaderClient.shouldGoToBackForwardListItem(this, item);
}
