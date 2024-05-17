void WebPageProxy::setTextFromItemForPopupMenu(WebPopupMenuProxy*, int32_t index)
{
    process()->send(Messages::WebPage::SetTextForActivePopupMenu(index), m_pageID);
}
