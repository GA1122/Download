void WebPageProxy::createNewPage(const WindowFeatures& windowFeatures, uint32_t opaqueModifiers, int32_t opaqueMouseButton, uint64_t& newPageID, WebPageCreationParameters& newPageParameters)
{
    RefPtr<WebPageProxy> newPage = m_uiClient.createNewPage(this, windowFeatures, static_cast<WebEvent::Modifiers>(opaqueModifiers), static_cast<WebMouseEvent::Button>(opaqueMouseButton));
    if (newPage) {
        newPageID = newPage->pageID();
        newPageParameters = newPage->creationParameters();
    } else
        newPageID = 0;
}
